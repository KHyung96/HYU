#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Graph* Graph;

struct _Graph
{
    int size;
    int* node;
    int** matrix;
};

Graph CreateGraph(int* nodes,int size);
void InsertEdge(Graph G, int a, int b);
void DeleteGraph(Graph G);

int main(int argc,char* argv[])
{
    FILE *in = fopen(argv[1],"r");
    int i,j,cnt=0,nodes[15];
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
    
    printf(" ");
    for(i=0; i<G->size; i++)
        printf(" %d",G->node[i]);
    printf("\n");
    for(i=0; i<G->size; i++)
    {
        printf("%d",G->node[i]);
        for(j=0; j<G->size; j++)
            printf(" %d",G->matrix[i][j]);
        printf("\n");
    }
    
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

