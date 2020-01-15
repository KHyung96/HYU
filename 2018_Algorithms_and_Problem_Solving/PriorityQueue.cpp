//2015004502_김형준_508
#include <stdio.h>

int c,num,ind,n,temp,cnt;
int heap[100005],output[100005];

void PercDown(int pos)
{
    if(pos*2 > n) return;
    if(heap[pos] < heap[pos*2])
    {
        if(pos*2+1 <= n && heap[pos*2+1] >= heap[pos*2])
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

void PercUp(int pos) {
    if(pos/2 < 1) return;
    if(heap[pos] > heap[pos/2])
    {
        temp=heap[pos];
        heap[pos]=heap[pos/2];
        heap[pos/2]=temp;
        PercUp(pos/2);
    }
}

int main()
{
    int i;
    while(1)
    {
        scanf("%d",&c);
        if(c == 0) break;
        else if(c == 1)
        {
            scanf("%d",&num);
            heap[++n]=num;
            PercUp(n);
        }
        else if(c == 2)
        {
            output[++cnt]=heap[1];
            temp=heap[1];
            heap[1]=heap[n];
            heap[n]=temp;
            n--;
            PercDown(1);
        }
        else if(c == 3)
        {
            scanf("%d %d",&ind,&num);
            if(heap[ind] > num)
            {
                heap[ind]=num;
                PercDown(ind);
            }
            else
            {
                heap[ind]=num;
                PercUp(ind);
            }
        }
    }
    for(i=1; i<=cnt; i++)
        printf("%d ",output[i]);
    printf("\n");
    for(i=1; i<=n; i++)
        printf("%d ",heap[i]);
    return 0;
}
