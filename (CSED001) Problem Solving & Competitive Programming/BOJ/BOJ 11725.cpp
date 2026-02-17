// BOJ 11725: 트리의 부모 찾기

#include <bits/stdc++.h>

using namespace std;

using ll=long long;

using pqig=priority_queue<int, vector<int>, greater<int>>;
using pqllg=priority_queue<long long, vector<long long>, greater<long long>>;

#define elif else if

const auto strnpos=string::npos;

template <typename TN1, typename TN2>
pair<TN1, TN2> operator+(const pair<TN1, TN2> &x, const pair<TN1, TN2> &y){
    return {x.first+y.first, x.second+y.second};
}

template <typename TN1, typename TN2>
pair<TN1, TN2> & operator+=(pair<TN1, TN2> &x, const pair<TN1, TN2> &y){
    x.first+=y.first; x.second+=y.second;
    return x;
}

// const ll mdls(1000000007LL);
// const int INF(INT_MAX/4);
// const int offset(500000);
// const array<pair<int, int>, 4> delta{{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};

int N;
vector<vector<int>> graph;
vector<bool> vst;
vector<int> parent;

void DFS(int cur){
    vst[cur]=true;
    for(int nxt: graph[cur]){
        if(!vst[nxt]){
            parent[nxt]=cur;
            DFS(nxt);
        }
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    cin>>N;
    graph.resize(N+1);
    vst.resize(N+1);
    parent.resize(N+1);
    for(int i(1); i<=N-1; ++i){
        int u, v; cin>>u>>v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    DFS(1);
    for(int i(2); i<=N; ++i){
        cout<<parent[i]<<'\n';
    }

    return 0;
}