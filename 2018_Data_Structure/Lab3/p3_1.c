#include <stdio.h>
#include <stdlib.h>
#define Header -1

typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;
typedef int ElementType;

struct Node
{
    ElementType element;
    Position next;
};

List MakeEmpty( List L );
int IsEmpty( List L );
int IsLast( Position P, List L );
void Insert( ElementType X, List L, Position P );
void PrintList(List L);

int main(int argc,char* argv[])
{
    ElementType X,Y;
    List L,Next;
    char inp;
    FILE *in = fopen(argv[1],"r");
    L=(List)malloc(sizeof(struct Node));
    L=MakeEmpty(L);
    while(!feof(in))
    {
        fscanf(in,"%c",&inp);
        if(inp == 'i')
        {
            fscanf(in,"%d %d",&X,&Y);
            Insert(X,L,L);
        }
        else if(inp == 'p')
		{
			if(IsEmpty(L)) printf("List is empty\n");
            else PrintList(L);
		}
    }
    while(!IsEmpty(L))
    {
        Next=L;
        L=L->next;
        free(Next);
    }
    return 0;
}

List MakeEmpty( List L )
{
    L=(List)malloc(sizeof(struct Node));
    L->element = Header;
    L->next = NULL;
    return L;
}

int IsEmpty( List L )
{
    return (L->next == NULL);
}

int IsLast( Position P, List L )
{
    return (P->next == NULL);
}

void Insert( ElementType X, List L, Position P )
{
    Position Temp;
    Temp=(Position)malloc(sizeof(struct Node));
    Temp->element=X;
    Temp->next=P->next;
    P->next=Temp;
}

void PrintList(List L)
{
    List P=L;
    while(!IsLast(P,L))
    {
        printf("key:%d   ",P->next->element);
        P=P->next;
    }
    printf("\n");
}

