//2015004502_김형준_508
#include <stdio.h>

int n,ix,iy,s,s1,s2,cnt;
int h[1005],check[1005];

int Find(int s)
{
    if(h[s] <= 0) return s;
    else return h[s]=Find(h[s]);
}

void Union(int s1,int s2)
{
    if(h[s1] > h[s2]) h[s1]=s2;
    else
    {
        if(h[s1] == h[s2]) h[s1]--;
        h[s2]=s1;
    }
}

int main()
{
    int i;
    scanf("%d",&n);
    while(!feof(stdin))
    {
        scanf("%d %d",&ix,&iy);
        s1=Find(ix);
        s2=Find(iy);
        if(s1 != s2)
            Union(s1,s2);
    }
    for(i=1; i<=n; i++)
    {
        s=Find(i);
        if(check[s] == 0) check[i]=check[s]=++cnt;
        else check[i]=check[s];
    }
    printf("%d\n",cnt);
    for(i=1; i<=n; i++)
        printf("%d\n",check[i]);
    return 0;
}
