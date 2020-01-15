//2015004502_김형준_508
#include <stdio.h>

int n,m,imsi,size,ans;
int path[60005];

struct code
{
    char name[5];
    int num;
    int ind;
}inp[30005],heap[30005],temp;

void PercDown(int pos)
{
    if(pos*2 > n) return;
    if(heap[pos].num > heap[pos*2].num)
    {
        if(pos*2+1 <= n && heap[pos*2+1].num < heap[pos*2].num)
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
    else if(pos*2+1 <= n && heap[pos].num > heap[pos*2+1].num)
    {
        temp=heap[pos];
        heap[pos]=heap[pos*2+1];
        heap[pos*2+1]=temp;
        PercDown(pos*2+1);
    }
}

void PercUp(int pos)
{
    if(pos/2 < 1) return;
    if(heap[pos].num < heap[pos/2].num)
    {
        temp=heap[pos];
        heap[pos]=heap[pos/2];
        heap[pos/2]=temp;
        PercUp(pos/2);
    }
}

void Delete()
{
    temp=heap[1];
    heap[1]=heap[n];
    heap[n]=temp;
    n--;
    PercDown(1);
}

int height(int pos)
{
    int cnt=0;
    while(path[pos])
    {
        cnt++;
        pos=path[pos];
    }
    return cnt;
}

int fixedLen(int a,int b)
{
    int i,cnt=0;
    for(i=2;; i*=2)
    {
        cnt++;
        if(i >= a) break;
    }
    return cnt*b;
}

int main()
{
    int i;
    scanf("%d",&n);
    imsi=n;
    for(i=1; i<=n; i++)
    {
        scanf("%s %d",heap[i].name,&heap[i].num);
        heap[i].ind=i;
        inp[i]=heap[i];
    }
    scanf("%d",&m);
    for(i=n/2; i>=1; i--)
        PercDown(i);
    size=n;
    while(n > 1)
    {
        Delete();
        Delete();
        n++;
        heap[n].name[0]=0;
        heap[n].num+=heap[n+1].num;
        size++;
        path[heap[n+1].ind]=size;
        path[heap[n].ind]=size;
        heap[n].ind=size;
        PercUp(n);
    }
    for(i=1; i<=imsi; i++)
        ans+=inp[i].num*height(i);
    printf("%d\n%d",fixedLen(imsi,m),ans);
    return 0;
}
