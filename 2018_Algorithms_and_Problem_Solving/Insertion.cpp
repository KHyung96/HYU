//2015004502_김형준_508

#include <stdio.h>

int n,num;
int inp[30005];

int main()
{
    int i,j;
    scanf("%d",&n);
    for(i=1; i<=n; i++)
        scanf("%d",&inp[i]);
    for(i=2; i<=n; i++)
    {
        num=inp[i];
        for(j=i-1; j>=1 && inp[j]<num; j--)
            inp[j+1]=inp[j];
        inp[j+1]=num;
    }
    for(i=1; i<=n; i++)
        printf("%d\n",inp[i]);
    return 0;
}
