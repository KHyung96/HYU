//2015004502_김형준_508

#include <stdio.h>

int n,inp[100005],mer[100005];

void Merge(int left,int mid,int right)
{
    int i,p1=left,p2=mid+1;
    int cnt=left-1;
    while(p1 <= mid && p2 <= right)
    {
        if(inp[p1] > inp[p2]) mer[++cnt]=inp[p1++];
        else mer[++cnt]=inp[p2++];
    }
    while(p1 <= mid) mer[++cnt]=inp[p1++];
    while(p2 <= right) mer[++cnt]=inp[p2++];
    for(i=left; i<=right; i++)
        inp[i]=mer[i];
}

void MergeSort(int left,int right)
{
    if(left < right)
    {
        int mid=(left+right)/2;
        MergeSort(left,mid);
        MergeSort(mid+1,right);
        Merge(left,mid,right);
    }
}

int main()
{
    int i;
    scanf("%d",&n);
    for(i=1; i<=n; i++)
        scanf("%d",&inp[i]);
    MergeSort(1,n);
    for(i=1; i<=n; i++)
        printf("%d\n",inp[i]);
    return 0;
}
