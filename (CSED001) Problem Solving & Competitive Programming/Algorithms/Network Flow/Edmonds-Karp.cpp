// Last Update on 20260405

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

struct EdmondsKarp{
    int N;
    vector<vector<int>> adj;
    vector<vector<int>> capacity;

    EdmondsKarp(int N): N(N), adj(N+1), capacity(N+1, vector<int>(N+1, 0)) {}

    void add_edge(int u, int v, int cap){
        adj[u].push_back(v);
        adj[v].push_back(u);
        capacity[u][v]+=cap;
    }

    int BFS(int s, int t, vector<int> &parent){
        parent[s]=0;
        deque<int> que;
        que.push_back(s);
        while(!que.empty()){
            int current(que.front()); que.pop_front();
            for(int next: adj[current]){
                if(parent[next]!=-1){
                    continue;
                }
                if(capacity[current][next]==0){
                    continue;
                }
                parent[next]=current;
                if(next==t){
                    int path_flow(INF);
                    int p(t);
                    while(p!=s){
                        int q(parent[p]);
                        path_flow=min(path_flow, capacity[q][p]);
                        p=q;
                    }
                    return path_flow;
                }
                que.push_back(next);
            }
        }
        return 0;
    }

    int get_max_flow(int s, int t){
        int max_flow(0);
        vector<int> parent(N+1);
        while(true){
            fill(parent.begin()+1, parent.end(), -1);
            int path_flow(BFS(s, t, parent));
            if(path_flow==0){
                break;
            }
            max_flow+=path_flow;
            int current(t);
            while(current!=s){
                int previous(parent[current]);
                capacity[previous][current]-=path_flow;
                capacity[current][previous]+=path_flow;
                current=previous;
            }
        }
        return max_flow;
    }
};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int N, M; cin>>N>>M;
    EdmondsKarp ek(N);
    for(int i(1); i<=M; ++i){
        int u, v, c; cin>>u>>v>>c;
        ek.add_edge(u, v, c);
    }
    int s, t; cin>>s>>t;
    cout<<ek.get_max_flow(s, t);

    return 0;
}