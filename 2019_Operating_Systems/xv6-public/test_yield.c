#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc,char* argv[])
{
    int pid;
    pid=fork();
    if(pid > 0)
    {
        while(1)
        {
            printf(1,"Parent\n");
            yield();
        }
    }
    else if(pid == 0)
    {
        while(1)
        {
            printf(1,"Child\n");
            yield();
        }
    }
    else
    {
        printf(2,"fork error\n");
        exit();
    }
    exit();
}
