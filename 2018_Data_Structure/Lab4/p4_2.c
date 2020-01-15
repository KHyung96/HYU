#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define Error 100000

typedef struct Stack
{
    int *key;
    int top;
    int max_stack_size;
}Stack;

Stack* CreateStack(int max);
void Push(Stack* S,int X);
int IsFull(Stack *S);
int IsEmpty(Stack *S);
int Pop(Stack* S);
void DeleteStack(Stack* S);
int Top(Stack* S);

int main(int argc,char* argv[])
{
	FILE *in = fopen(argv[1],"r");	
    Stack* stack;
    char input_str[101];
    int max,i=0,a,b,result=0;
    fgets(input_str,101,in);
    max=strlen(input_str);
    printf("Top numbers : ");
    stack=CreateStack(max);
    while(i < max && input_str[i] != '#')
    {
        if(input_str[i] >= '0' && input_str[i] <= '9') Push(stack,input_str[i]-'0');
        else
        {
            b=Pop(stack);
            a=Pop(stack);
			if(a != Error && b != Error)
			{
            	if(input_str[i] == '+') result=a+b;
            	else if(input_str[i] == '-') result=a-b;
            	else if(input_str[i] == '*') result=a*b;
            	else if(input_str[i] == '/') result=a/b;
            	else if(input_str[i] == '%') result=a%b;
            	Push(stack,result);
			}
        }
		i++;
    }
	printf("\nevaluation result : ");
	if(stack->top == 0) printf("%d\n",Pop(stack));
	else if(IsEmpty(stack)) printf("Evaluate Error : Stack is empty\n");
	else printf("Evaluate Error : Stack have numbers yet\n");
    DeleteStack(stack);
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
		printf("%d ",X);
	}
}

int IsFull(Stack* S)
{
    return (S->top == S->max_stack_size-1);
}

int IsEmpty(Stack* S)
{
    return (S->top == -1);
}

int Pop(Stack* S)
{
    if(IsEmpty(S))
	{
		printf("Pop Error : Stack is Empty ");
		return Error;
	}
    int top_number=Top(S);
	S->top--;
    return top_number;
}

void DeleteStack(Stack* S)
{
    free(S->key);
    free(S);
}

int Top(Stack* S)
{
	return S->key[S->top];
}
