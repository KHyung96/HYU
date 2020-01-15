//2015004502_김형준_508
#include <stdio.h>
#include <string.h>

int la,lb,cnt,px,py;
int dy[505][505],path[505][505];
char a[505],b[505],ans[505];

int main()
{
    int i,j;
    scanf("%s %s",a+1,b+1);
    la=py=(int)strlen(a+1);
    lb=px=(int)strlen(b+1);
    for(i=1; i<=lb; i++)
    {
        for(j=1; j<=la; j++)
        {
            if(a[j] == b[i])
            {
                dy[i][j]=dy[i-1][j-1]+1;
                path[i][j]=1;
            }
            else
            {
                if(dy[i-1][j] >= dy[i][j-1])
                {
                    dy[i][j]=dy[i-1][j];
                    path[i][j]=2;
                }
                else
                {
                    dy[i][j]=dy[i][j-1];
                    path[i][j]=3;
                }
            }
        }
    }
    while(!(px == 0 || py == 0))
    {
        if(path[px][py] == 1)
        {
            ans[++cnt]=a[py];
            px--;
            py--;
        }
        else if(path[px][py] == 2) px--;
        else py--;
    }
    for(i=cnt; i>=1; i--)
        printf("%c",ans[i]);
    return 0;
}
