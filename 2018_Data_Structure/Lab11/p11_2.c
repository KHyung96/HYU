#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Graph* Graph;
typedef struct _Queue* Queue;

struct _Graph
{
    int size;
    int* node;
    int** matrix;
};

struct _Queue
{
    int* key;
    int first;
    int rear;
    int qsize;
    int max_queue_size;
};

Graph CreateGraph(int* nodes,int size);
void InsertEdge(Graph G,int a,int b);
void DeleteGraph(Graph G);
void Topsort(Graph G);
Queue MakeNewQueue(int X);
void Enqueue(Queue Q,int X);
int Dequeue(Queue Q);
int IsEmpty(Queue Q);
int IsFull(Queue Q);
void DeleteQueue(Queue Q);
void MakeEmpty(Queue Q);

int main(int argc,char* argv[])
{
    FILE *in = fopen(argv[1],"r");
    int i,cnt=0,nodes[105]={0,};
    char inp[105];
    
    fgets(inp,sizeof(inp),in);
    for(i=0; i<strlen(inp); i++)
        if(inp[i] >= '0' && inp[i] <= '9')
            nodes[cnt++]=inp[i]-'0';
    Graph G;
    G=CreateGraph(nodes,cnt);
    
    fgets(inp,sizeof(inp),in);
    for(i=0; i<strlen(inp); i++)
        if(inp[i] == '-')
            InsertEdge(G,inp[i-1]-'0',inp[i+1]-'0');
    
    Topsort(G);
    DeleteGraph(G);
    fclose(in);
    return 0;
}

Graph CreateGraph(int* nodes,int size)
{
    int i,j;
    Graph G;
    G=(Graph)malloc(sizeof(struct _Graph));
    G->size=size;
    G->node=(int*)malloc(sizeof(int)*G->size);
    for(i=0; i<G->size; i++)
        G->node[i]=nodes[i];
    G->matrix=(int**)malloc(sizeof(int*)*G->size);
    for(i=0; i<G->size; i++)
    {
        G->matrix[i]=(int*)malloc(sizeof(int)*G->size);
        for(j=0; j<G->size; j++)
            G->matrix[i][j]=0;
    }
    return G;
}

void InsertEdge(Graph G, int a, int b)
{
    int i,pa,pb;
    for(i=0; i<G->size; i++)
    {
        if(G->node[i] == a) pa=i;
        if(G->node[i] == b) pb=i;
    }
    G->matrix[pa][pb]=1;
}

void DeleteGraph(Graph G)
{
    int i;
    for(i=0; i<G->size; i++)
        free(G->matrix[i]);
    free(G->matrix);
    free(G->node);
    free(G);
}

void Topsort(Graph G)
{
    int i,j,num,temp;
    int list[15],cnt=0;
    Queue Q;
    Q=MakeNewQueue(G->size);
    for(i=0; i<G->size; i++)
    {
        for(j=0; j<G->size; j++)
            if(G->matrix[j][i] == 1)
                break;
        if(j == G->size)
            list[cnt++]=i;
    }
    for(i=0; i<cnt; i++)
    {
        for(j=i+1; j<cnt; j++)
        {
            if(G->node[list[i]] > G->node[list[j]])
            {
                temp=list[i];
                list[i]=list[j];
                list[j]=temp;
            }
        }
    }
    for(i=0; i<cnt; i++)
        Enqueue(Q,list[i]);
    while(!IsEmpty(Q))
    {
        num=Dequeue(Q);
        printf("%d ",G->node[num]);
        cnt=0;
        for(i=0; i<G->size; i++)
        {
            if(G->matrix[num][i] == 1)
            {
                G->matrix[num][i]=0;
                for(j=0; j<G->size; j++)
                    if(G->matrix[j][i] == 1)
                        break;
                if(j == G->size)
                    list[cnt++]=i;
            }
        }
        for(i=0; i<cnt; i++)
        {
            for(j=i+1; j<cnt; j++)
            {
                if(G->node[list[i]] > G->node[list[j]])
                {
                    temp=list[i];
                    list[i]=list[j];
                    list[j]=temp;
                }
            }
        }
        for(i=0; i<cnt; i++)
            Enqueue(Q,list[i]);
    }
    printf("\n");
    DeleteQueue(Q);
}

Queue MakeNewQueue(int X)
{
    Queue Q;
    Q=(Queue)malloc(sizeof(struct _Queue));
    Q->key=(int*)malloc(sizeof(int)*X);
    Q->max_queue_size=X;
    MakeEmpty(Q);
    return Q;
}

void Enqueue(Queue Q, int X)
{
    if(IsFull(Q))
    {
        printf("Error : Queue is full.\n");
        return;
    }
    Q->qsize++;
    Q->key[++Q->rear]=X;
}

int Dequeue(Queue Q)
{
    if(IsEmpty(Q))
    {
        printf("Error : Queue is empty.\n");
        return -1;
    }
    Q->qsize--;
    return Q->key[++Q->first];
}

int IsEmpty(Queue Q)
{
    return (Q->qsize == 0);
}

int IsFull(Queue Q)
{
    return (Q->rear == Q->max_queue_size-1);
}

void DeleteQueue(Queue Q)
{
    free(Q->key);
    free(Q);
}

void MakeEmpty(Queue Q)
{
    Q->first=Q->rear=-1;
    Q->qsize=0;
}

