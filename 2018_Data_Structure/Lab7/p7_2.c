#include <stdio.h>
#include <stdlib.h>

struct HeapStruct;
typedef int ElementType;
typedef struct HeapStruct Heap;

struct HeapStruct
{
    int Capacity;
    int Size;
    ElementType* Elements;
};

int IsFull(Heap* heap);
int IsEmpty(Heap* heap);
Heap* CreateHeap(int heapSize);
void Insert(Heap* heap, int value);
int Find (Heap* heap, int value);
int DeleteMax(Heap* heap);
void PrintHeap(Heap* heap);

int main(int argc,char* argv[])
{
    FILE *in = fopen(argv[1],"r");
    char cv;
    Heap* maxHeap=NULL;
    int heapSize,key;
    while(!feof(in))
    {
        fscanf(in,"%c",&cv);
        if(cv == 'n')
        {
            fscanf(in,"%d",&heapSize);
            maxHeap=CreateHeap(heapSize);
        }
        else if(cv == 'i')
        {
            fscanf(in,"%d",&key);
            Insert(maxHeap,key);
        }
        else if(cv == 'd') DeleteMax(maxHeap);
        else if(cv == 'p') PrintHeap(maxHeap);
        else if(cv == 'f')
        {
            fscanf(in,"%d",&key);
            if(Find(maxHeap,key)) printf("%d is in the tree.\n",key);
            else printf("%d is not in the tree.\n",key);
        }
    }
    free(maxHeap->Elements);
    free(maxHeap);
    fclose(in);
    return 0;
}

int IsFull(Heap* heap)
{
	return (heap->Size == heap->Capacity);
}

int IsEmpty(Heap* heap)
{
	return (heap->Size == 0);
}

Heap* CreateHeap(int heapSize)
{
    Heap* newHeap;
    newHeap=(Heap*)malloc(sizeof(struct HeapStruct));
    newHeap->Elements=(ElementType*)malloc(sizeof(ElementType)*(heapSize+1));
    newHeap->Capacity=heapSize;
    newHeap->Size=0;
	newHeap->Elements[0]=0x7fffffff;
    return newHeap;
}

void Insert(Heap* heap, int value)
{
    int i;
    if(IsFull(heap))
    {
        printf("Insertion Error : Max Heap is full.\n");
        return;
    }
    if(Find(heap,value))
    {
        printf("%d is already in the tree.\n",value);
        return;
    }
    heap->Size++;
    for(i=heap->Size; value > heap->Elements[i/2]; i/=2)
        heap->Elements[i]=heap->Elements[i/2];
    heap->Elements[i]=value;
    printf("insert %d\n",value);
}

int Find (Heap* heap, int value)
{
    int i;
    for(i=1; i<=heap->Size; i++)
        if(heap->Elements[i] == value)
            return 1;
    return 0;
}

int DeleteMax(Heap* heap)
{
    if(IsEmpty(heap))
    {
        printf("Deletion Error : Max Heap is empty.\n");
        return 0;
    }
    int i,NextChild;
    ElementType MaxElement=heap->Elements[1];
    ElementType LastElement=heap->Elements[heap->Size];
    heap->Size--;
    for(i=1; i*2<=heap->Size; i=NextChild)
    {
        NextChild=i*2;
        if(NextChild+1 <= heap->Size && heap->Elements[NextChild+1] > heap->Elements[NextChild])
            NextChild++;
        if(LastElement > heap->Elements[NextChild]) break;
        else heap->Elements[i]=heap->Elements[NextChild];
	}
    heap->Elements[i]=LastElement;
    return MaxElement;
}

void PrintHeap(Heap* heap)
{
    int i;
    if(IsEmpty(heap))
    {
        printf("Print Error : Max Heap is empty.\n");
        return;
    }
    for(i=1; i<=heap->Size; i++)
        printf("%d ",heap->Elements[i]);
    printf("\n");
}

