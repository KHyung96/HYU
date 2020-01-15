#include <stdio.h>
#include <stdlib.h>

int main(int argc,char** argv)
{
    int i;
    int numberOfNames;
    char** names;
    printf("enter %s names:\n",argv[1]);
    numberOfNames=atoi(argv[1]);
    names=(char**)malloc(sizeof(char*)*numberOfNames);
    for(i=0; i<numberOfNames; i++)
    {
        *(names+i)=(char*)malloc(sizeof(char)*30);
        scanf("%s",*(names+i));
    }
    printf("the names you entered:\n");
    for(i=0; i<numberOfNames; i++)
        printf("%s\n",*(names+i));
    free(names);
    return 0;
}

