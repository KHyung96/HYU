//2015004502_김형준_508
#include <stdio.h>

int n,m,k,inp,min;
int query[100005][2],check[100005];

int main()
{
    int i;
    scanf("%d %d %d",&n,&m,&k);
    for(i=1; i<=k; i++)
        scanf("%d %d",&query[i][0],&query[i][1]);
    min=m+1;
    for(i=1; i<=n; i++)
    {
        scanf("%d",&inp);
        check[inp]++;
        if(min > inp) min=inp;
    }
    for(i=min; i<=m; i++)
        check[i]+=check[i-1];
    for(i=1; i<=k; i++)
    {
        if(query[i][0] > query[i][1]) printf("0\n");
        else printf("%d\n",check[query[i][1]]-check[query[i][0]-1]);
    }
    return 0;
}
