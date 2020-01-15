#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"

int
getppid(void)
{
    return myproc()->parent->pid;
}

int
sys_getppid(void)
{
    return getppid();
}
