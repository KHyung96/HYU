//2015004502_김형준_508
#include <stdio.h>

int n,m,inp,ans;
bool check[100005];

int main()
{
    int i;
    scanf("%d %d",&n,&m);
    for(i=1; i<=n; i++)
    {
        scanf("%d",&inp);
        check[inp]=true;
    }
    for(i=1; i<=m; i++)
    {
        scanf("%d",&inp);
        if(check[inp]) ans++;
    }
    printf("%d",ans);
    return 0;
}
