#include <stdio.h>

void swap(int *num1,int *num2)
{
    int temp;
    temp=*num1;
    *num1=*num2;
    *num2=temp;
}

int main(int argv,char** argc)
{
    int num1,num2;
	
    printf("enter 2 integers\n");
    scanf("%d %d",&num1,&num2);

    printf("you entered\n%d, %d\n",num1,num2);
    swap(&num1,&num2);
    printf("after swapping\n%d, %d\n",num1,num2);
	return 0;
}
