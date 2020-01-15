#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"

struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

static struct proc *initproc;

int nextpid = 1;
int nexttid = 1;
extern void forkret(void);
extern void trapret(void);

static void wakeup1(void *chan);

void
pinit(void)
{
  initlock(&ptable.lock, "ptable");
}

// Must be called with interrupts disabled
int
cpuid() {
  return mycpu()-cpus;
}

// Must be called with interrupts disabled to avoid the caller being
// rescheduled between reading lapicid and running through the loop.
struct cpu*
mycpu(void)
{
  int apicid, i;
  
  if(readeflags()&FL_IF)
    panic("mycpu called with interrupts enabled\n");
  
  apicid = lapicid();
  // APIC IDs are not guaranteed to be contiguous. Maybe we should have
  // a reverse map, or reserve a register to store &cpus[i].
  for (i = 0; i < ncpu; ++i) {
    if (cpus[i].apicid == apicid)
      return &cpus[i];
  }
  panic("unknown apicid\n");
}

// Disable interrupts so that we are not rescheduled
// while reading proc from the cpu structure
struct proc*
myproc(void) {
  struct cpu *c;
  struct proc *p;
  pushcli();
  c = mycpu();
  p = c->proc;
  popcli();
  return p;
}

//PAGEBREAK: 32
// Look in the process table for an UNUSED proc.
// If found, change state to EMBRYO and initialize
// state required to run in the kernel.
// Otherwise return 0.
static struct proc*
allocproc(void)
{
  struct proc *p;
  char *sp;

  acquire(&ptable.lock);

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    if(p->state == UNUSED)
      goto found;

  release(&ptable.lock);
  return 0;

found:
  p->state = EMBRYO;
  p->pid = nextpid++;

  // Initial queue level, priority, monopolize
  p->lev = p->priority = p->mono = p->quantum = 0;
  // Make this process is not thread
  p->isthread  = p->tid = p->msize = 0;
  memset(&p->tmem, 0, sizeof p->tmem);

  release(&ptable.lock);

  // Allocate kernel stack.
  if((p->kstack = kalloc()) == 0){
    p->state = UNUSED;
    return 0;
  }
  sp = p->kstack + KSTACKSIZE;

  // Leave room for trap frame.
  sp -= sizeof *p->tf;
  p->tf = (struct trapframe*)sp;

  // Set up new context to start executing at forkret,
  // which returns to trapret.
  sp -= 4;
  *(uint*)sp = (uint)trapret;

  sp -= sizeof *p->context;
  p->context = (struct context*)sp;
  memset(p->context, 0, sizeof *p->context);
  p->context->eip = (uint)forkret;

  return p;
}

//PAGEBREAK: 32
// Set up first user process.
void
userinit(void)
{
  struct proc *p;
  extern char _binary_initcode_start[], _binary_initcode_size[];

  p = allocproc();
  
  initproc = p;
  if((p->pgdir = setupkvm()) == 0)
    panic("userinit: out of memory?");
  inituvm(p->pgdir, _binary_initcode_start, (int)_binary_initcode_size);
  p->sz = PGSIZE;
  memset(p->tf, 0, sizeof(*p->tf));
  p->tf->cs = (SEG_UCODE << 3) | DPL_USER;
  p->tf->ds = (SEG_UDATA << 3) | DPL_USER;
  p->tf->es = p->tf->ds;
  p->tf->ss = p->tf->ds;
  p->tf->eflags = FL_IF;
  p->tf->esp = PGSIZE;
  p->tf->eip = 0;  // beginning of initcode.S

  safestrcpy(p->name, "initcode", sizeof(p->name));
  p->cwd = namei("/");

  // this assignment to p->state lets other cores
  // run this process. the acquire forces the above
  // writes to be visible, and the lock is also needed
  // because the assignment might not be atomic.
  acquire(&ptable.lock);

  p->state = RUNNABLE;

  release(&ptable.lock);
}

// Grow current process's memory by n bytes.
// Return 0 on success, -1 on failure.
int
growproc(int n)
{
  uint sz;
  struct proc *curproc = myproc();
  struct proc *p;

  acquire(&ptable.lock);
  p = curproc;
  if(curproc->isthread == 1)
    p = curproc->parent;

  sz = p->sz;
  if(n > 0){
    if((sz = allocuvm(curproc->pgdir, sz, sz + n)) == 0){
      release(&ptable.lock);
      return -1;
    }
  } else if(n < 0){
    if((sz = deallocuvm(curproc->pgdir, sz, sz + n)) == 0){
      release(&ptable.lock);
      return -1;
    }
  }
  p->sz = sz;
  release(&ptable.lock);

  switchuvm(curproc);
  return 0;
}

// Create a new process copying p as the parent.
// Sets up stack to return as if from system call.
// Caller must set state of returned proc to RUNNABLE.
int
fork(void)
{
  int i, pid;
  struct proc *np;
  struct proc *curproc = myproc();

  // Allocate process.
  if((np = allocproc()) == 0){
    return -1;
  }

  if(curproc->isthread == 1){
    np->pgdir = copyuvm(curproc->pgdir, curproc->parent->sz);
  }
  else{
    np->pgdir = copyuvm(curproc->pgdir, curproc->sz);
  }

  // Copy process state from proc.

  if(np->pgdir == 0){
    kfree(np->kstack);
    np->kstack = 0;
    np->state = UNUSED;
    return -1;
  }
  np->sz = curproc->sz;
  np->parent = curproc;
  *np->tf = *curproc->tf;

  // Clear %eax so that fork returns 0 in the child.
  np->tf->eax = 0;

  for(i = 0; i < NOFILE; i++)
    if(curproc->ofile[i])
      np->ofile[i] = filedup(curproc->ofile[i]);
  np->cwd = idup(curproc->cwd);

  safestrcpy(np->name, curproc->name, sizeof(curproc->name));

  pid = np->pid;

  acquire(&ptable.lock);

  np->state = RUNNABLE;

  release(&ptable.lock);

  return pid;
}

// Exit the current process.  Does not return.
// An exited process remains in the zombie state
// until its parent calls wait() to find out it exited.
void
exit(void)
{
  struct proc *p;
  struct proc *curproc = myproc();
  int fd, havekids;

  if(curproc == initproc)
    panic("init exiting");

  if(curproc->isthread == 0){
    acquire(&ptable.lock);

    for(;;){
      havekids = 0;
      for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
        if(p->isthread == 1 && p->parent == curproc){
          if(p->state == ZOMBIE)
            dealloc_thread(p);
          else{
            havekids = 1;
            p->killed = 1;
            wakeup1(p);
          }
        }
      }
      if(havekids == 0){
        release(&ptable.lock);
        break;
      }
      sleep(curproc, &ptable.lock);
    }
  }

  // Close all open files.
  for(fd = 0; fd < NOFILE; fd++){
    if(curproc->ofile[fd]){
      fileclose(curproc->ofile[fd]);
      curproc->ofile[fd] = 0;
    }
  }

  begin_op();
  iput(curproc->cwd);
  end_op();
  curproc->cwd = 0;

  acquire(&ptable.lock);

  // Parent might be sleeping in wait().
  if(curproc->isthread == 1)
    curproc->parent->killed = 1;
  wakeup1(curproc->parent);

  // Pass abandoned childeren to init.
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->parent == curproc){
      p->parent = initproc;
      if(curproc->state == ZOMBIE)
        wakeup1(initproc);
    }
  }

  // Jump into the scheduler, never to return.
  curproc->state = ZOMBIE;
  sched();
  panic("zombie exit");
}

// Wait for a child process to exit and return its pid.
// Return -1 if this process has no children.
int
wait(void)
{
  struct proc *p;
  int havekids, pid;
  struct proc *curproc = myproc();
  
  acquire(&ptable.lock);
  for(;;){
    // Scan through table looking for exited children.
    havekids = 0;
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->parent != curproc)
        continue;
      havekids = 1;
      if(p->state == ZOMBIE){
        // Found one.
        pid = p->pid;
        kfree(p->kstack);
        p->kstack = 0;
        freevm(p->pgdir);
        p->pid = 0;
        p->parent = 0;
        p->name[0] = 0;
        p->killed = 0;
        p->state = UNUSED;
        release(&ptable.lock);
        return pid;
      }
    }

    // No point waiting if we don't have any children.
    if(!havekids || curproc->killed){
      release(&ptable.lock);
      return -1;
    }

    // Wait for children to exit.  (See wakeup1 call in proc_exit.)
    sleep(curproc, &ptable.lock);  //DOC: wait-sleep
  }
}

//PAGEBREAK: 42
// Per-CPU process scheduler.
// Each CPU calls scheduler() after setting itself up.
// Scheduler never returns.  It loops, doing:
//  - choose a process to run
//  - swtch to start running that process
//  - eventually that process transfers control
//      via swtch back to the scheduler.
void
scheduler(void)
{
  struct proc *p;
  struct cpu *c = mycpu();
  c->proc = 0;
  
  for(;;){
    // Enable interrupts on this processor.
    sti();

#ifdef FCFS_SCHED
    acquire(&ptable.lock);
    struct proc *nextp = 0;
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->state != RUNNABLE)
          continue;

      if(nextp){
        if(nextp->pid > p->pid)
          nextp = p;
      }
      else
        nextp = p;
    }
    if(nextp){
      c->proc = nextp;
      switchuvm(nextp);
      nextp->state = RUNNING;
      nextp->quantum = 0;

      swtch(&(c->scheduler), nextp->context);
      switchkvm();

      c->proc = 0;
    }
    release(&ptable.lock);

#elif MLFQ_SCHED
    acquire(&ptable.lock);
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->state != RUNNABLE || p->lev != 0)
        continue;

      c->proc = p;
      switchuvm(p);
      p->state = RUNNING;
      p->quantum = 0;

      swtch(&(c->scheduler), p->context);
      switchkvm();

      c->proc = 0;
    }
    int check = 0;
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->state != RUNNABLE || p->lev != 0)
        continue;
      check = 1;
    }
    if(check == 0){
      struct proc *nextp = 0;
      for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
        if(p->state != RUNNABLE || p->lev != 1)
          continue;

        if(nextp){
          if(nextp->priority <= p->priority){
            if(nextp->priority == p->priority && nextp->pid > p->pid)
              nextp = p;
            else if(nextp->priority < p->priority)
              nextp = p;
          }
        }
        else
          nextp = p;
      }
      if(nextp){
        c->proc = nextp;
        switchuvm(nextp);
        nextp->state = RUNNING; 
        nextp->quantum = 0;

        swtch(&(c->scheduler), nextp->context);
        switchkvm();

        c->proc = 0;
      }
    }
    release(&ptable.lock);

#else
    // Loop over process table looking for process to run.
    acquire(&ptable.lock);
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->state != RUNNABLE)
        continue;

      // Switch to chosen process.  It is the process's job
      // to release ptable.lock and then reacquire it
      // before jumping back to us.
      c->proc = p;
      switchuvm(p);
      p->state = RUNNING;

      swtch(&(c->scheduler), p->context);
      switchkvm();

      // Process is done running for now.
      // It should have changed its p->state before coming back.
      c->proc = 0;
    }
    release(&ptable.lock);

#endif
  }
}

// Enter scheduler.  Must hold only ptable.lock
// and have changed proc->state. Saves and restores
// intena because intena is a property of this
// kernel thread, not this CPU. It should
// be proc->intena and proc->ncli, but that would
// break in the few places where a lock is held but
// there's no process.
void
sched(void)
{
  int intena;
  struct proc *p = myproc();

  if(!holding(&ptable.lock))
    panic("sched ptable.lock");
  if(mycpu()->ncli != 1)
    panic("sched locks");
  if(p->state == RUNNING)
    panic("sched running");
  if(readeflags()&FL_IF)
    panic("sched interruptible");
  intena = mycpu()->intena;
  swtch(&p->context, mycpu()->scheduler);
  mycpu()->intena = intena;
}

// Give up the CPU for one scheduling round.
void
yield(void)
{
  acquire(&ptable.lock);  //DOC: yieldlock
  myproc()->state = RUNNABLE;
  sched();
  release(&ptable.lock);
}

// A fork child's very first scheduling by scheduler()
// will swtch here.  "Return" to user space.
void
forkret(void)
{
  static int first = 1;
  // Still holding ptable.lock from scheduler.
  release(&ptable.lock);

  if (first) {
    // Some initialization functions must be run in the context
    // of a regular process (e.g., they call sleep), and thus cannot
    // be run from main().
    first = 0;
    iinit(ROOTDEV);
    initlog(ROOTDEV);
  }

  // Return to "caller", actually trapret (see allocproc).
}

// Atomically release lock and sleep on chan.
// Reacquires lock when awakened.
void
sleep(void *chan, struct spinlock *lk)
{
  struct proc *p = myproc();
  
  if(p == 0)
    panic("sleep");

  if(lk == 0)
    panic("sleep without lk");

  // Must acquire ptable.lock in order to
  // change p->state and then call sched.
  // Once we hold ptable.lock, we can be
  // guaranteed that we won't miss any wakeup
  // (wakeup runs with ptable.lock locked),
  // so it's okay to release lk.
  if(lk != &ptable.lock){  //DOC: sleeplock0
    acquire(&ptable.lock);  //DOC: sleeplock1
    release(lk);
  }
  // Go to sleep.
  p->chan = chan;
  p->state = SLEEPING;

  sched();

  // Tidy up.
  p->chan = 0;

  // Reacquire original lock.
  if(lk != &ptable.lock){  //DOC: sleeplock2
    release(&ptable.lock);
    acquire(lk);
  }
}

//PAGEBREAK!
// Wake up all processes sleeping on chan.
// The ptable lock must be held.
static void
wakeup1(void *chan)
{
  struct proc *p;

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    if(p->state == SLEEPING && p->chan == chan)
      p->state = RUNNABLE;
}

// Wake up all processes sleeping on chan.
void
wakeup(void *chan)
{
  acquire(&ptable.lock);
  wakeup1(chan);
  release(&ptable.lock);
}

// Kill the process with the given pid.
// Process won't exit until it returns
// to user space (see trap in trap.c).
int
kill(int pid)
{
  struct proc *p;
  int retval = -1;

  acquire(&ptable.lock);
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->pid == pid && p->tid == 0){
      p->killed = 1;
      // Wake process from sleep if necessary.
      if(p->state == SLEEPING)
        p->state = RUNNABLE;
      retval = 0;
    }
  }
  release(&ptable.lock);
  return retval;
}

//PAGEBREAK: 36
// Print a process listing to console.  For debugging.
// Runs when user types ^P on console.
// No lock to avoid wedging a stuck machine further.
void
procdump(void)
{
  static char *states[] = {
  [UNUSED]    "unused",
  [EMBRYO]    "embryo",
  [SLEEPING]  "sleep ",
  [RUNNABLE]  "runble",
  [RUNNING]   "run   ",
  [ZOMBIE]    "zombie"
  };
  int i;
  struct proc *p;
  char *state;
  uint pc[10];

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->state == UNUSED)
      continue;
    if(p->state >= 0 && p->state < NELEM(states) && states[p->state])
      state = states[p->state];
    else
      state = "???";
    cprintf("%d %s %s", p->pid, state, p->name);
    if(p->state == SLEEPING){
      getcallerpcs((uint*)p->context->ebp+2, pc);
      for(i=0; i<10 && pc[i] != 0; i++)
        cprintf(" %p", pc[i]);
    }
    cprintf("\n");
  }
}

int
getlev(void)
{
  return myproc()->lev;
}

void
setpriority(int pid,int priority)
{
  if(priority < 0 || priority > 10){
    cprintf("setpriority: Priority must be between 1 and 10\n");
    myproc()->killed = 1;
    return;
  }

  struct proc *p;

  acquire(&ptable.lock);
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->pid == pid){
      p->priority = priority;
      release(&ptable.lock);
      return;
    }
  }
  release(&ptable.lock);
  cprintf("setpriority: No processor with pid = %d\n",pid);
}

void
monopolize(int password)
{
#ifdef MLFQ_SCHED
  if(password != 2015004502){
    cprintf("monopolize: Wrong password\n");
    myproc()->killed = 1;
    return;
  }
  if(myproc()->mono)
    myproc()->lev = myproc()->priority = myproc()->quantum = myproc()->mono = 0;
  else
    myproc()->mono = 1;

#else
  cprintf("monopolize: Change to MLFQ Scheduler\n");
  return;

#endif
}

void
upqueue(void)
{
  struct proc *p;

  acquire(&ptable.lock);
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    p->lev = p->priority = p->quantum = 0;
  }
  release(&ptable.lock);
}

void
chpreempt(void)
{
  struct proc *p;
  struct proc *nextp = myproc();

  acquire(&ptable.lock);
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->state != RUNNABLE || p->lev != 1)
      continue;
    if(nextp->priority <= p->priority){
      if(nextp->priority == p->priority && nextp->pid > p->pid)
        nextp = p;
      else if(nextp->priority < p->priority)
        nextp = p;
    }
  }
  release(&ptable.lock);
  if(myproc()->pid != nextp->pid)
    yield();
}

int
thread_create(thread_t *thread, void *(*start_routine)(void*), void *arg)
{
  int i;
  uint sz, sp, ustack[2];
  pde_t *pgdir;
  struct proc *np;
  struct proc *curproc = myproc();

  if((np = allocproc()) == 0){
    return -1;
  }
  nextpid--;

  if(curproc->isthread == 1)
    curproc = curproc->parent;

  np->parent = curproc;
  np->isthread = 1;
  np->pid = curproc->pid;
  np->tid = nexttid++;
  *thread = np->tid;

  acquire(&ptable.lock);

  pgdir = curproc->pgdir;
  if(curproc->msize > 0)
    np->stbase = curproc->tmem[--curproc->msize];
  else{
    np->stbase = curproc->sz;
    curproc->sz += 2 * PGSIZE;
  }

  if((sz = allocuvm(pgdir, np->stbase, np->stbase + 2 * PGSIZE)) == 0){
    np->state = UNUSED;
    return -1;
  }

  release(&ptable.lock);

  np->pgdir = pgdir;
  np->sz = sz;

  *np->tf = *curproc->tf;
  np->tf->eax = 0;
  np->tf->eip = (int)start_routine;

  ustack[0] = 0xffffffff;
  ustack[1] = (uint)arg;
  sp = sz - 8;
  if(copyout(np->pgdir, sp, ustack, 8) < 0)
    return -1;
  np->tf->esp = sp;

  for(i = 0; i < NOFILE; i++)
    if(curproc->ofile[i])
      np->ofile[i] = filedup(curproc->ofile[i]);
  np->cwd = idup(curproc->cwd);

  safestrcpy(np->name, curproc->name, sizeof(curproc->name));

  acquire(&ptable.lock);

  np->state = RUNNABLE;

  release(&ptable.lock);

  return 0;
}

void
thread_exit(void *retval)
{
  struct proc *curproc = myproc();
  int fd;

  if(curproc == initproc)
    panic("init exiting");

  for(fd = 0; fd < NOFILE; fd++){
    if(curproc->ofile[fd]){
      fileclose(curproc->ofile[fd]);
      curproc->ofile[fd] = 0;
    }
  }

  begin_op();
  iput(curproc->cwd);
  end_op();
  curproc->cwd = 0;

  acquire(&ptable.lock);

  wakeup1(curproc->parent);

  curproc->state = ZOMBIE;
  curproc->retval = retval;
  sched();
  panic("zombie exit");
}

int
thread_join(thread_t thread, void **retval)
{
  struct proc *p;
  struct proc *curproc = myproc();

  acquire(&ptable.lock);
  for(;;){
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->tid != thread)
        continue;

      if(p->parent != curproc){
        release(&ptable.lock);
        return -1;
      }

      if(p->state == ZOMBIE){
        *retval = p->retval;
        dealloc_thread(p);
        release(&ptable.lock);
        return 0;
      }
    }

    if(curproc->killed){
      release(&ptable.lock);
      return -1;
    }

    sleep(curproc, &ptable.lock);
  }
}

void
kill_threads(int pid, struct proc *curproc)
{
  struct proc *p;

  acquire(&ptable.lock);
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->pid == pid && p != curproc){
      p->killed = 1;
      p->chan = 0;
      p->state = SLEEPING;
    }
  }
  release(&ptable.lock);
}

void
dealloc_thread(struct proc *p)
{
  p->parent->tmem[p->parent->msize++] = p->stbase;
  kfree(p->kstack);
  p->kstack = 0;
  p->pid = 0;
  p->parent = 0;
  p->name[0] = 0;
  p->killed = 0;
  p->tid = 0;
  p->isthread = 0;
  p->state = UNUSED;
  deallocuvm(p->pgdir, p->sz, p->stbase);
}

void
wakeup2(int pid, struct proc *curproc)
{
  struct proc *p;

  acquire(&ptable.lock);

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->pid == pid && p != curproc){
      p->state = RUNNABLE;
      p->parent = curproc;
    }
  }
  release(&ptable.lock);
  wait();
}
