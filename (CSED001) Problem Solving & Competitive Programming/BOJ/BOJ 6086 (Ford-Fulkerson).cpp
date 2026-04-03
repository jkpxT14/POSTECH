// BOJ 6086: Total Flow(최대 유량)

#include <bits/stdc++.h>

using namespace std;

using ll=long long;
using ull=unsigned long long;

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

const int INF(INT_MAX);

struct FordFulkerson{
    int N;
    vector<vector<int>> adj;
    vector<vector<int>> capacity;
    vector<bool> visited;

    FordFulkerson(int N): N(N), adj(N+1), capacity(N+1, vector<int>(N+1, 0)), visited(N+1, false) {}

    void add_edge(int u, int v, int cap){
        adj[u].push_back(v);
        adj[v].push_back(u);
        capacity[u][v]+=cap;
        capacity[v][u]+=cap;
    }

    int DFS(int current, int t, int limit){
        if(current==t){
            return limit;
        }
        visited[current]=true;
        for(int next: adj[current]){
            if(visited[next]){
                continue;
            }
            if(capacity[current][next]==0){
                continue;
            }
            int path_flow(DFS(next, t, min(limit, capacity[current][next])));
            if(path_flow>0){
                capacity[current][next]-=path_flow;
                capacity[next][current]+=path_flow;
                return path_flow;
            }
        }
        return 0;
    }

    int get_max_flow(int s, int t){
        int max_flow(0);
        while(true){
            fill(visited.begin()+1, visited.end(), false);
            int path_flow(DFS(s, t, INF));
            if(path_flow==0){
                break;
            }
            max_flow+=path_flow;
        }
        return max_flow;
    }
};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    auto convert = [](char c) -> int {
        if('A'<=c && c<='Z') return c-'A'+1;
        if('a'<=c && c<='z') return c-'a'+27;
        return 0;
    };

    int N; cin>>N;
    FordFulkerson ff(52);
    for(int i(1); i<=N; ++i){
        char u, v;
        int c;
        cin>>u>>v>>c;
        ff.add_edge(convert(u), convert(v), c);
    }
    cout<<ff.get_max_flow(1, 26);

    return 0;
}