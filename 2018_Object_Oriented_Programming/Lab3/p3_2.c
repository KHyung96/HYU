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
void Delete( ElementType X, List L );
Position Find( ElementType X, List);
Position FindPrevious ( ElementType X, List L );
void DeleteList ( List L );

int main(int argc,char* argv[])
{
    ElementType X,Y;
    List L;
    Position P;
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
            P=Find(Y,L);
            if(P == NULL)  printf("Insertion(%d) Failed : cannot find the location to be inserted\n",X);
            else Insert(X,L,P);
        }
        else if(inp == 'p')
        {
            if(IsEmpty(L)) printf("List is empty\n");
            else PrintList(L);
        }
        else if(inp == 'd')
        {
            fscanf(in,"%d",&X);
            Delete(X,L);
        }
        else if(inp == 'f')
        {
            fscanf(in,"%d",&X);
            P=Find(X,L);
            if(P == NULL) printf("Could not find %d in the list\n",X);
            else
            {
                P=FindPrevious(X,L);
                printf("Key of the previous node of %d is ",X);
                if(P->element == Header) printf("header\n");
                else printf("%d\n",P->element);
            }
        }
    }
    DeleteList(L);
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

void Delete( ElementType X, List L )
{
    Position P,Temp;
    P=FindPrevious(X,L);
    if(Find(X,L) == NULL)
    {
        printf("Deletion failed : element %d is not in the list\n",X);
        return;
    }
    if(!IsLast(P,L))
    {
        Temp=P->next;
        P->next=Temp->next;
        free(Temp);
    }
}

Position Find( ElementType X, List L )
{
    List P=L;
    while(P != NULL)
    {
        if(P->element == X)
            return P;
        P=P->next;
    }
    return NULL;
}

Position FindPrevious ( ElementType X, List L )
{
    Position P=L;
    while(P->next != NULL && P->next->element != X)
        P=P->next;
    return P;
}

void DeleteList ( List L )
{
    Position P, Temp;
    P=L->next;
    L->next=NULL;
    while(!IsEmpty(P))
    {
        Temp=P->next;
        free(P);
        P=Temp;
    }
}

