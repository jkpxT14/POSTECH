// BOJ 17616, JUNGOL 3428: 등수 찾기(ranking)

#include <bits/stdc++.h>

using namespace std;

using ll=long long;
using ull=unsigned long long;

#define elif else if

const auto strnpos(string::npos);

template <typename TN1, typename TN2>
pair<TN1, TN2> operator +(const pair<TN1, TN2> &x, const pair<TN1, TN2> &y){
    return {x.first+y.first, x.second+y.second};
}

template <typename TN1, typename TN2>
pair<TN1, TN2> &operator +=(pair<TN1, TN2> &x, const pair<TN1, TN2> &y){
    x.first+=y.first; x.second+=y.second;
    return x;
}

// const ll mdls(1000000007LL);
// const int INF(INT_MAX/4);
// const int offset(500000);
// const array<pair<int, int>, 4> delta{{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};

vector<vector<int>> adj, radj;
vector<bool> visited;

int dfs(int cur){
    int ret(1);
    visited[cur]=true;
    for(int next: adj[cur]){
        if(!visited[next]){
            ret+=dfs(next);
        }
    }
    return ret;
}

int rdfs(int cur){
    int ret(1);
    visited[cur]=true;
    for(int next: radj[cur]){
        if(!visited[next]){
            ret+=rdfs(next);
        }
    }
    return ret;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int N, M, X; cin>>N>>M>>X;
    adj.resize(N+1); radj.resize(N+1);
    for(int i(1); i<=M; ++i){
        int A, B; cin>>A>>B;
        adj[B].push_back(A);
        radj[A].push_back(B);
    }
    visited.assign(N+1, false);
    int U(dfs(X));
    visited.assign(N+1, false);
    int V(N-rdfs(X)+1);
    cout<<U<<' '<<V;

    return 0;
}