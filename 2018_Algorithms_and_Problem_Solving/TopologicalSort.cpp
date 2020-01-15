//2015004502_김형준_508
#include <stdio.h>

int n,ix,iy,cnt,cycle;
int inp[1005][1005];
int visit[1005],check[1005],ans[1005];

void dfs(int p)
{
    int i;
    visit[p]=1;
    for(i=1; i<=n; i++)
    {
        if(inp[p][i] == 1)
        {
            if(visit[i] == 0) dfs(i);
            else if(check[i] == 0) cycle=1;
        }
    }
    check[p]=1;
    ans[++cnt]=p;
}

int main()
{
    int i;
    scanf("%d",&n);
    while(!feof(stdin))
    {
        scanf("%d %d",&ix,&iy);
        inp[ix][iy]=1;
    }
    for(i=1; i<=n; i++)
    {
        if(visit[i] == 0)
            dfs(i);
    }
    if(cycle == 1) printf("0");
    else
    {
        printf("1\n");
        for(i=n; i>=1; i--)
            printf("%d ",ans[i]);
    }
    return 0;
}
