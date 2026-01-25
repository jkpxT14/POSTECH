#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>

using namespace std;

using ll=long long;

int parent[1000001], Rank[1000001]; // Union-by-Rank

int Find(int v)
{
    if(v==parent[v]){
        return v;
    }
    return parent[v]=Find(parent[v]); // Path compression
}

void Union(int u, int v)
{
    u=Find(u);
    v=Find(v);
    if(u==v){
        return;
    }
    if(Rank[u]==Rank[v]){
        parent[v]=u;
        Rank[u]++;
        return;
    }
    else if(Rank[u]>Rank[v]){
        parent[v]=u;
        return;
    }
    parent[u]=v;
    return;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin>>n>>m;
    for(int i=0; i<=n; i++){
        parent[i]=i;
        Rank[i]=1;
    }
    for(int i=0; i<m; i++){
        int c, a, b;
        cin>>c>>a>>b;
        if(c==0){
            Union(a, b);
        }
        else if(c==1){
            if(Find(a)==Find(b)){
                cout<<"YES"<<"\n";
            }
            else{
                cout<<"NO"<<"\n";
            }
        }
    }
    // [Study] return 0;
}