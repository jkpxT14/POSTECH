// BOJ 1916: 최소비용 구하기

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

    int N; cin>>N;
    vector<vector<pair<int, int>>> graph(N+1);
    int M; cin>>M;
    for(int i(0); i<=M-1; ++i){
        int from, to, cost; cin>>from>>to>>cost;
        graph[from].push_back({to, cost});
    }
    int initial, terminal; cin>>initial>>terminal;
    vector<int> dist(N+1, INF);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    dist[initial]=0;
    pq.push({0, initial});
    while(!pq.empty()){
        pair<int, int> cur(pq.top()); pq.pop();
        if(dist[cur.second]<cur.first){
            continue;
        }
        for(pair<int, int> bus: graph[cur.second]){
            pair<int, int> nxt({cur.first+bus.second, bus.first});
            if(dist[nxt.second]>nxt.first){
                dist[nxt.second]=nxt.first;
                pq.push(nxt);
            }
        }
    }
    cout<<dist[terminal];

    return 0;
}