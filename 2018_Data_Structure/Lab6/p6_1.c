#include <stdio.h>
#include <stdlib.h>

struct AVLNode;
typedef struct AVLNode* AVLTree;
typedef int ElementType;

struct AVLNode
{
    ElementType Element;
    AVLTree Left;
    AVLTree Right;
    int Height;
};

int Max(int a,int b);
int Height(AVLTree T);
AVLTree Insert(ElementType X,AVLTree T);
void PrintInorder(AVLTree T);
void DeleteTree(AVLTree T);

int main(int argc,char **argv)
{
    FILE *in = fopen(argv[1],"r");
    AVLTree myTree = NULL;
    int num;
    
    if(in == NULL)
    {
        printf("There is no file : %s\n",argv[1]);
        exit(-1);
    }
    while(fscanf(in,"%d",&num) != EOF)
    {
        myTree = Insert(num,myTree);
        PrintInorder(myTree);
        printf("\n");
    }
    DeleteTree(myTree);
	fclose(in);
	return 0;
}

int Max(int a,int b)
{
	if(a > b) return a;
	return b;
}

int Height(AVLTree T)
{
	if(T == NULL) return -1;
	return T->Height;
}

AVLTree Insert(ElementType X,AVLTree T)
{
    if(T == NULL)
    {
        T=(AVLTree)malloc(sizeof(struct AVLNode));
        T->Element=X;
        T->Height=0;
        T->Left=T->Right=NULL;
    }
    else if(X < T->Element)
        T->Left=Insert(X,T->Left);
    else if(X > T->Element)
        T->Right=Insert(X,T->Right);
    else if(X == T->Element)
        printf("[Error] %d already in the tree!\n",X);
    T->Height=Max(Height(T->Left),Height(T->Right))+1;
	return T;
}

void PrintInorder(AVLTree T)
{
    if(T == NULL) return;
	PrintInorder(T->Left);
    printf("%d(%d) ",T->Element,T->Height);
    PrintInorder(T->Right);
}

void DeleteTree(AVLTree T)
{
    if(T == NULL) return;
    DeleteTree(T->Left);
    DeleteTree(T->Right);
    free(T);
}

