//2015004502_김형준_508
#include <stdio.h>
#include <queue>
using namespace std;

int n,ix,iy,iz,cnt;
int s1,s2,h[1005];
priority_queue<pair<int,pair<int,int> > > q;


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
        scanf("%d %d %d",&ix,&iy,&iz);
        if(ix < iy) q.push({-iz,{-ix,-iy}});
        else q.push({-iz,{-iy,-ix}});
    }
    printf("%d\n",n-1);
    while(!q.empty())
    {
        ix=-q.top().second.first;
        iy=-q.top().second.second;
        iz=-q.top().first;
        q.pop();
        s1=Find(ix);
        s2=Find(iy);
        if(s1 != s2)
        {
            cnt++;
            Union(s1,s2);
            printf("%d %d %d\n",ix,iy,iz);
            if(cnt == n-1) break;
        }
    }
    return 0;
}
