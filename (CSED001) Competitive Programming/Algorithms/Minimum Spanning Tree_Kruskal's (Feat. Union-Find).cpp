// Baekjoon Online Judge Prob. 1197

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <tuple>
#include <limits>

using namespace std;

using ll=long long;
using vi=vector<int>;
using pii=pair<int, int>;
#define INT_MAX 2147483647

vector<int> parent(10001);

int find(int v)
{
    if(v==parent[v]) return v;
    else return parent[v]=find(parent[v]);
}

void union_(int u, int v)
{
    u=find(u);
    v=find(v);
    if(u>v) parent[u]=v;
    else parent[v]=u;
    return;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int V, E, ans_weight=0;
    cin>>V>>E;
    parent.resize(V+1);
    for(int i=1; i<=V; i++){
        parent[i]=i;
    }
    vector<pair<int, pair<int, int>>> edge(E), MST;
    for(int i=0; i<E; i++){
        int A, B, C;
        cin>>A>>B>>C;
        edge[i]=make_pair(C, make_pair(A, B));
    }
    sort(edge.begin(), edge.end());
    for(int i=0; i<E; i++){
        if(MST.size()==V-1){
            cout<<ans_weight;
            return 0;
        }
        if(find(edge[i].second.first)==find(edge[i].second.second)) continue;
        else{
            MST.push_back(edge[i]);
            union_(edge[i].second.first, edge[i].second.second);
            ans_weight+=edge[i].first;
        }
    }
    cout<<ans_weight;
    return 0;
}