#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Stack
{
    int *key;
    int top;
    int max_stack_size;
}Stack;

Stack* CreateStack(int max);
void Push(Stack* S,int X);
int IsFull(Stack *S);

int main(int argc,char* argv[])
{
	FILE *in = fopen(argv[1],"r");
    Stack* stack;
    char input_str[101];
    int max,i=0;
    fgets(input_str,101,in);
    max=strlen(input_str);
    stack=CreateStack(max);
    while(i < max && input_str[i] != '#')
    {
        if(input_str[i] >= '0' && input_str[i] <= '9')
			Push(stack,input_str[i]-'0');
		i++;
    }
	free(stack->key);
	free(stack);
	fclose(in);
    return 0;
}

Stack* CreateStack(int max)
{
    Stack* S=NULL;
    S=(Stack*)malloc(sizeof(Stack));
    S->key=(int*)malloc(sizeof(int)*max);
    S->max_stack_size=max;
    S->top=-1;
    return S;
}

void Push(Stack* S,int X)
{
    if(IsFull(S)) printf("Push Error : Stack is full ");
    else
    {
        S->key[++S->top]=X;
        printf("%d inserted\n",X);
    }
}

int IsFull(Stack* S)
{
    return (S->top == S->max_stack_size-1);
}

