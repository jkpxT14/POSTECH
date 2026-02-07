// BOJ 11437, 11438: LCA 1, 2

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

int N, max_jump;
vector<vector<int>> adj;
vector<vector<int>> parent;
vector<int> depth;

void build_tree(int cur){
    for(int nxt: adj[cur]){
        if(depth[nxt]==-1){
            parent[nxt][0]=cur;
            depth[nxt]=depth[cur]+1;
            build_tree(nxt);
        }
    }
}

void find_parent(){
    for(int i(1); i<=max_jump; ++i){
        for(int j(1); j<=N; ++j){
            parent[j][i]=parent[parent[j][i-1]][i-1];
        }
    }
}

int lift_node(int node, int dist){
    for(int i(0); i<=max_jump; ++i){
        if(dist&(1<<i)){
            node=parent[node][i];
        }
    }
    return node;
}

int LCA(int u, int v){
    if(depth[u]<depth[v]){
        swap(u, v);
    }
    u=lift_node(u, depth[u]-depth[v]);
    if(u==v){
        return u;
    }
    for(int i(max_jump); i>=0; --i){
        if(parent[u][i]!=parent[v][i]){
            u=parent[u][i];
            v=parent[v][i];
        }
    }
    return parent[u][0];
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    cin>>N;
    adj.resize(N+1);
    max_jump=int(floor(log2(N-1)));
    parent.resize(N+1, vector<int>(max_jump+1, 0));
    depth.resize(N+1, -1);
    for(int i(1); i<=N-1; ++i){
        int u, v; cin>>u>>v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    depth[1]=0;
    build_tree(1);
    find_parent();
    int M; cin>>M;
    while(M--){
        int u, v; cin>>u>>v;
        cout<<LCA(u, v)<<'\n';
    }

    return 0;
}