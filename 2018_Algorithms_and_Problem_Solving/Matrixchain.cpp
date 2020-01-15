//2015004502_김형준_508
#include <stdio.h>

int n,inp[105];
int dy[105][105],pos[105][105];
int parcnt[105][2];

void make_answer(int st,int fi)
{
    if(st == fi) return;
    else
    {
        parcnt[st][0]++;
        make_answer(st,pos[st][fi]);
        make_answer(pos[st][fi]+1,fi);
        parcnt[fi][1]++;
    }
}

int main()
{
    int p,i,j,k;
    scanf("%d",&n);
    for(i=0; i<=n; i++)
        scanf("%d",&inp[i]);
    for(p=2; p<=n; p++)
    {
        for(i=1; i<=n-p+1; i++)
        {
            j=p+i-1;
            dy[i][j]=0x7fffffff;
            for(k=i; k<j; k++)
            {
                if(dy[i][j] > dy[i][k]+dy[k+1][j]+inp[i-1]*inp[k]*inp[j])
                {
                    dy[i][j]=dy[i][k]+dy[k+1][j]+inp[i-1]*inp[k]*inp[j];
                    pos[i][j]=k;
                }
            }
        }
    }
    printf("%d\n",dy[1][n]);
    make_answer(1,n);
    for(i=1; i<=n; i++)
    {
        for(j=1; j<=parcnt[i][0]; j++)
            printf("( ");
        printf("%d ",i);
        for(j=1; j<=parcnt[i][1]; j++)
            printf(") ");
    }
    return 0;
}
