#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define MAX 1005

void
errmsg(char msg[])
{
    fprintf(stderr,"%s error\n",msg);
    exit(-1);
}

void
forknexec(char cmd[])
{
    int i,lcnt,pcnt=0,wpid,pid[MAX];
    char *token_s,*token_b,*list[MAX];
    char *s,*b;
    token_s=strtok_r(cmd,";",&s);
    while(token_s != NULL)
    {
        token_b=strtok_r(token_s," ",&b);
        lcnt=0;
        while(token_b != NULL)
        {
            list[lcnt++]=token_b;
            token_b=strtok_r(NULL," ",&b);
        }
        list[lcnt]=NULL;
        pid[pcnt]=fork();
        if(pid[pcnt] == 0)
        {
            if(execvp(list[0],list) == -1)
                errmsg(strcat(list[0]," exec"));
        }
        else if(pid[pcnt] < 0)
            errmsg("fork");
        pcnt++;
        token_s=strtok_r(NULL,";",&s);
    }
    for(i=0; i<pcnt; i++)
    {
        wpid=-1;
        while(wpid != pid[i])
        {
            wpid=waitpid(pid[i],NULL,0);
            if(wpid == -1)
                errmsg("waitpid");
        }
    }
}

int
main(int argc, char* argv[])
{
    FILE *fp;
    if(argc == 1) fp=stdin;  // interactive mode
    else if(argc == 2) fp=fopen(argv[1],"r");  // batch mode
    else errmsg("argument");
    if(fp == NULL)
        errmsg("fopen");
    char cmd[MAX];
    while(!feof(fp))
    {
        if(argc == 1) fprintf(stdout,"prompt> ");
        if(fgets(cmd,sizeof(cmd),fp) == NULL) break;
        if(cmd[strlen(cmd)-1] == '\n') cmd[strlen(cmd)-1]=0;
        if(argc == 2) fprintf(stdout,"%s\n",cmd);
        if(strcmp(cmd,"quit") == 0) break;
        forknexec(cmd);
    }
    fclose(fp);
    exit(0);
}

