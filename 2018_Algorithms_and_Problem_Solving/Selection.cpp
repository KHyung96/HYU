//2015004502_김형준_508
#include <stdio.h>

int n,m,min,pos;
int inp[30005];

int main()
{
    int i,j,temp;
    scanf("%d %d",&n,&m);
    for(i=1; i<=n; i++)
        scanf("%d",&inp[i]);
    for(i=1; i<=m; i++)
    {
        min=0x7fffffff;
        for(j=i; j<=n; j++)
        {
            if(min > inp[j])
            {
                min=inp[j];
                pos=j;
            }
        }
        temp=inp[i];
        inp[i]=inp[pos];
        inp[pos]=temp;
    }
    for(i=1; i<=n; i++)
        printf("%d\n",inp[i]);
    return 0;
}
