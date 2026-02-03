// Last Update on 20260203

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
const int INF(INT_MAX);
// const int offset(500000);
// const array<pair<int, int>, 4> delta{{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int V, E; cin>>V>>E;
    vector<vector<pair<int, int>>> graph(V+1);
    for(int i(1); i<=E; ++i){
        int u, v, w; cin>>u>>v>>w;
        graph[u].push_back({v, w});
        // graph[v].push_back({u, w});
    }
    vector<int> dist(V+1, INF);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    int K; cin>>K;
    dist[K]=0;
    pq.push({0, K});
    while(!pq.empty()){
        pair<int, int> cur(pq.top()); pq.pop();
        if(dist[cur.second]<cur.first){
            continue;
        }
        for(pair<int, int> edge: graph[cur.second]){
            pair<int, int> nxt({cur.first+edge.second, edge.first});
            if(dist[nxt.second]>nxt.first){
                dist[nxt.second]=nxt.first;
                pq.push(nxt);
            }
        }
    }
    for(int i(1); i<=V; ++i){
        if(dist[i]==INF){
            cout<<"INF\n";
        } else{
            cout<<dist[i]<<'\n';
        }
    }

    return 0;
}