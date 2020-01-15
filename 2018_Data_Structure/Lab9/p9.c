#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct DisjointSet
{
    int size_maze;
    int *ptr_arr;
}DisjointSets;

void Init(DisjointSets *sets,DisjointSets *maze_print,int num);
void Union(DisjointSets *sets,int i,int j);
int Find(DisjointSets *sets,int i);
void CreateMaze(DisjointSets *sets,DisjointSets *maze_print,int num);
void PrintMaze(DisjointSets *sets,int num);
void FreeMaze(DisjointSets *sets,DisjointSets *maze_print);

int main(int argc,char* argv[])
{
    int num;
	FILE *in = fopen(argv[1],"r");
    DisjointSets *sets,*maze_print;
    fscanf(in,"%d",&num);
    sets=(DisjointSets*)malloc(sizeof(DisjointSets));
    maze_print=(DisjointSets*)malloc(sizeof(DisjointSets));
    Init(sets,maze_print,num);
    CreateMaze(sets,maze_print,num);
    PrintMaze(maze_print,num);
    FreeMaze(sets,maze_print);
	fclose(in);
	return 0;
}

void Init(DisjointSets *sets,DisjointSets *maze_print,int num)
{
    int i;
    sets->size_maze=num*num;
    sets->ptr_arr=(int*)malloc(sizeof(int)*(sets->size_maze+1));
    for(i=1; i<=sets->size_maze; i++)
        sets->ptr_arr[i]=0;
    maze_print->size_maze=sets->size_maze*2;
    maze_print->ptr_arr=(int*)malloc(sizeof(int)*(maze_print->size_maze));
    for(i=0; i<maze_print->size_maze; i++)
        maze_print->ptr_arr[i]=-1; // i번째칸은 (i-1)*2 : 동 (i-1)*2+1 : 남
	maze_print->ptr_arr[maze_print->size_maze-2]=0;
}

void Union(DisjointSets *sets,int i,int j)
{
    if(sets->ptr_arr[i] > sets->ptr_arr[j])
        sets->ptr_arr[i]=j;
    else
    {
        if(sets->ptr_arr[i] == sets->ptr_arr[j])
            sets->ptr_arr[i]--;
        sets->ptr_arr[j]=i;
    }
}

int Find(DisjointSets *sets,int i)
{
    if(sets->ptr_arr[i] <= 0) return i;
    return sets->ptr_arr[i]=Find(sets,sets->ptr_arr[i]);
}

void CreateMaze(DisjointSets *sets,DisjointSets *maze_print,int num)
{
    int x,y;
    srand((unsigned int)time(NULL));
    while(Find(sets,1) != Find(sets,sets->size_maze))
    {
        x=rand()%(sets->size_maze)+1;
        y=rand()%2;
        if(y == 0 && x%num != 0 && Find(sets,x) != Find(sets,x+1))
        {
            maze_print->ptr_arr[(x-1)*2]=0;
            Union(sets,Find(sets,x),Find(sets,x+1));
        }
        else if(y == 1 && x+num <= sets->size_maze && Find(sets,x) != Find(sets,x+num))
        {
            maze_print->ptr_arr[(x-1)*2+1]=0;
            Union(sets,Find(sets,x),Find(sets,x+num));
        }
    }
}

void PrintMaze(DisjointSets *sets,int num)
{
    int i,j,pos;
    for(i=1; i<=num; i++)
        printf(" -");
    printf("\n");
    for(i=1; i<=num; i++)
    {
        if(i == 1) printf(" ");
        else printf("|");
        for(j=1; j<=num; j++)
        {
            pos=(i-1)*num+j;
            if(sets->ptr_arr[(pos-1)*2] == -1) printf(" |");
            else printf("  ");
        }
        printf("\n");
        for(j=1; j<=num; j++)
        {
            pos=(i-1)*num+j;
            if(sets->ptr_arr[(pos-1)*2+1] == -1) printf(" -");
            else printf("  ");
        }
        printf("\n");
    }
}

void FreeMaze(DisjointSets *sets,DisjointSets *maze_print)
{
    free(sets->ptr_arr);
    free(sets);
    free(maze_print->ptr_arr);
    free(maze_print);
}

