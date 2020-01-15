//2015004502_김형준_508
#include <stdio.h>

int n,k,size,temp;
int heap[100005];

void PercDown(int pos)
{
    if(pos*2 > n) return;
    if(heap[pos] < heap[pos*2])
    {
        if(pos*2+1 <= n && heap[pos*2+1] > heap[pos*2])
        {
            temp=heap[pos];
            heap[pos]=heap[pos*2+1];
            heap[pos*2+1]=temp;
            PercDown(pos*2+1);
        }
        else
        {
            temp=heap[pos];
            heap[pos]=heap[pos*2];
            heap[pos*2]=temp;
            PercDown(pos*2);
        }
    }
    else if(pos*2+1 <= n && heap[pos] < heap[pos*2+1])
    {
        temp=heap[pos];
        heap[pos]=heap[pos*2+1];
        heap[pos*2+1]=temp;
        PercDown(pos*2+1);
    }
}

int main()
{
    int i;
    scanf("%d %d",&n,&k);
    for(i=1; i<=n; i++)
        scanf("%d",&heap[i]);
    for(i=n/2; i>=1; i--)
        PercDown(i);
    for(i=1; i<=n && k>=1; k--)
    {
        printf("%d ",heap[1]);
        temp=heap[1];
        heap[1]=heap[n];
        heap[n]=temp;
        n--;
        PercDown(1);
    }
    printf("\n");
    for(i=1; i<=n; i++)
        printf("%d ",heap[i]);
    return 0;
}
