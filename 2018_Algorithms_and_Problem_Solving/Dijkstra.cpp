//2015004502_김형준_508
#include <stdio.h>
#include <vector>
#include <queue>
using namespace std;

int n,m,ix,iy,iz,ans;
int p[5005],dis,pos;
vector<pair<int,int> > inp[5005];
priority_queue<pair<int,int> > q;

int main()
{
    int i,j;
    scanf("%d",&n);
    for(i=1; i<=n; i++)
    {
        scanf("%d %d",&ix,&m);
        for(j=1; j<=m; j++)
        {
            scanf("%d %d",&iy,&iz);
            inp[ix].push_back(make_pair(iy,iz));
        }
    }
    for(i=1; i<=n; i++)
        p[i]=0x7fffffff;
    p[1]=0;
    q.push(make_pair(0,1));
    do
    {
        dis=-q.top().first;
        pos=q.top().second;
        q.pop();
        for(i=0; i<inp[pos].size(); i++)
        {
            if(p[inp[pos][i].first] > p[pos]+inp[pos][i].second)
            {
                p[inp[pos][i].first] = p[pos]+inp[pos][i].second;
                q.push(make_pair(-p[inp[pos][i].first],inp[pos][i].first));
            }
        }
    }while(!q.empty());
    for(i=1; i<=n; i++)
        if(p[i] != 0x7fffffff && ans < p[i])
            ans=p[i];
    printf("%d",ans);
    return 0;
}
