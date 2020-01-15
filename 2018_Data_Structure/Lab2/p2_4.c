#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define INT_MAX 2147438647

bool IsZero(int x)
{
    return x?0:1;
}

bool Equal(int x,int y)
{
    return (x==y)?1:0;
}

int Successor(int x)
{
    return (x<INT_MAX)?(x+1):x;
}

int Add(int x,int y)
{
	long long int add = (long long int)x+y;
    return (add<INT_MAX)?(add):INT_MAX;
}

int Subtract(int x,int y)
{
    return (x>y)?(x-y):0;
}

int main(int argc,char **args)
{
    int x = atoi(args[1]);
    int y = atoi(args[2]);
    
    printf("Is %d zero? %s\n",x,IsZero(x)?"True":"False");
    printf("Is %d zero? %s\n",y,IsZero(y)?"True":"False");
    printf("Does %d equal %d? %s\n",x,y,Equal(x,y)?"True":"False");
    printf("%d's next number is %d\n",x,Successor(x));
    printf("%d's next number is %d\n",y,Successor(y));
    printf("%d + %d = %d\n",x,y,Add(x,y));
    printf("%d - %d = %d\n",x,y,Subtract(x,y));
    
    return 0;
}
