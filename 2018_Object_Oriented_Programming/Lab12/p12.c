#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INF 999999999

typedef struct _Graph* Graph;
typedef struct _Heap* Heap;

typedef struct Node
{
    int vertex;
    int dist;
    int prev;
}Node;

struct _Graph
{
    int size;
    int** vertices;
    Node* nodes;
};

struct _Heap
{
    int Capacity;
    int Size;
    Node* Element;
};

Graph CreateGraph(int size);
void PrintShortestPath(Graph g);
Heap CreateMinHeap(int heapSize);
void InsertToMinHeap(Heap minheap,int vertex,int distance);
Node DeleteMin(Heap minheap);

int main(int argc,char* argv[])
{
    FILE *in = fopen(argv[1],"r");
    Graph g;
    int size;
    fscanf(in,"%d\n",&size);
    g=CreateGraph(size+1);
    while(!feof(in))
    {
        int node1,node2,weight;
        fscanf(in,"%d-%d-%d ",&node1,&node2,&weight);
        g->vertices[node1][node2]=weight;
    }
    PrintShortestPath(g);
    fclose(in);
    return 0;
}

Graph CreateGraph(int size)
{
    Graph g;
    g=(Graph)malloc(sizeof(struct _Graph));
    g->size=size-1;
    g->vertices=(int**)malloc(sizeof(int*)*size);
    int i,j;
    for(i=1; i<=g->size; i++)
    {
        g->vertices[i]=(int*)malloc(sizeof(int)*size);
        for(j=1; j<=g->size; j++)
            g->vertices[i][j]=INF;
    }
    g->nodes=(Node*)malloc(sizeof(Node)*size);
    return g;
}

void PrintShortestPath(Graph g)
{
    int i,j;
    for(i=1; i<=g->size; i++)
    {
        g->nodes[i].vertex=g->nodes[i].prev=i;
        g->nodes[i].dist=INF;
    }
    g->nodes[1].dist=0;
    Heap minheap;
    minheap=CreateMinHeap(g->size+1);
    InsertToMinHeap(minheap,1,0);
    Node now;
    while(minheap->Size != 0)
    {
        now=DeleteMin(minheap);
        for(i=1; i<=g->size; i++)
        {
            if(g->nodes[i].dist > g->nodes[now.vertex].dist+g->vertices[now.vertex][i])
            {
                g->nodes[i].dist=g->nodes[now.vertex].dist+g->vertices[now.vertex][i];
                g->nodes[i].prev=now.vertex;
                InsertToMinHeap(minheap,i,g->nodes[i].dist);
            }
        }
    }
    int p,cnt;
    int path[g->size];
    for(i=2; i<=g->size; i++)
    {
        if(g->nodes[i].dist == INF)
        {
            printf("No way to %d vertex.\n",i);
            continue;
        }
        p=i;
        cnt=0;
        while(p != 1)
        {
            path[cnt++]=p;
            p=g->nodes[p].prev;
        }
        printf("1");
        for(j=cnt-1; j>=0; j--)
            printf("->%d",path[j]);
        printf(" (cost : %d)\n",g->nodes[i].dist);
    }
}

Heap CreateMinHeap(int heapSize)
{
    Heap h;
    h=(Heap)malloc(sizeof(struct _Heap));
    h->Capacity=heapSize;
    h->Size=0;
    h->Element=(Node*)malloc(sizeof(Node)*heapSize);
    h->Element[0].dist=-1;
    return h;
}

void InsertToMinHeap(Heap minheap,int vertex,int distance)
{
    int i;
    minheap->Size++;
    for(i=minheap->Size; distance < minheap->Element[i/2].dist; i/=2)
        minheap->Element[i]=minheap->Element[i/2];
    minheap->Element[i].vertex=vertex;
    minheap->Element[i].dist=distance;
}

Node DeleteMin(Heap minheap)
{
    int i,NextChild;
    Node MinElement=minheap->Element[1];
    Node LastElement=minheap->Element[minheap->Size];
    minheap->Size--;
    for(i=1; i*2<=minheap->Size; i=NextChild)
    {
        NextChild=i*2;
        if(NextChild+1 <= minheap->Size && minheap->Element[NextChild+1].dist < minheap->Element[NextChild].dist)
            NextChild++;
        if(LastElement.dist < minheap->Element[NextChild].dist) break;
        else minheap->Element[i]=minheap->Element[NextChild];
    }
    minheap->Element[i]=LastElement;
    return MinElement;
}

