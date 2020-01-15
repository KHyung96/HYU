//2015004502_김형준_508
#include <stdio.h>

int n,inp[105],dy[105],path[105];

int main()
{
    int i,j;
    scanf("%d",&n);
    for(i=1; i<=n; i++)
        scanf("%d",&inp[i]);
    for(i=1; i<=n; i++)
    {
        for(j=1; j<=i; j++)
        {
            if(dy[i] < inp[j]+dy[i-j])
            {
                dy[i]=inp[j]+dy[i-j];
                path[i]=j;
            }
        }
    }
    printf("%d\n",dy[n]);
    while(n)
    {
        printf("%d ",path[n]);
        n=n-path[n];
    }
    return 0;
}
