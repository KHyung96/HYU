//2015004502_김형준_508
#include <stdio.h>

int n,cnt,ans[105];

struct lines
{
    int l1;
    int l2;
}inp[105],line[105],path[105],prev,now;

int Min(int a,int b)
{
    if(a < b) return a;
    return b;
}

int MinPos(int a,int b)
{
    if(a <= b) return 1;
    return 2;
}

int main()
{
    int i;
    scanf("%d",&n);
    scanf("%d %d %d %d",&prev.l1,&prev.l2,&line[n+1].l1,&line[n+1].l2);
    for(i=1; i<=n; i++) scanf("%d",&inp[i].l1);
    for(i=1; i<=n; i++) scanf("%d",&inp[i].l2);
    for(i=2; i<=n; i++) scanf("%d",&line[i].l1);
    for(i=2; i<=n; i++) scanf("%d",&line[i].l2);
    prev.l1+=inp[1].l1;
    prev.l2+=inp[1].l2;
    now=prev;
    for(i=2; i<=n; i++)
    {
        now.l1=Min(prev.l1+inp[i].l1,prev.l2+inp[i].l1+line[i].l2);
        path[i].l1=MinPos(prev.l1+inp[i].l1,prev.l2+inp[i].l1+line[i].l2);
        now.l2=Min(prev.l2+inp[i].l2,prev.l1+inp[i].l2+line[i].l1);
        path[i].l2=MinPos(prev.l1+inp[i].l2+line[i].l1,prev.l2+inp[i].l2);
        prev=now;
    }
    now.l1+=line[n+1].l1;
    now.l2+=line[n+1].l2;
    cnt=n;
    ans[cnt]=MinPos(now.l1,now.l2);
    while(cnt > 1)
    {
        cnt--;
        if(ans[cnt+1] == 1) ans[cnt]=path[cnt+1].l1;
        else ans[cnt]=path[cnt+1].l2;
    }
    printf("%d\n",Min(now.l1,now.l2));
    for(i=1; i<=n; i++)
        printf("%d %d\n",ans[i],i);
    return 0;
}
