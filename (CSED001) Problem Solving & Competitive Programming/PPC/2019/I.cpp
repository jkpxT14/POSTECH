// BOJ 17242: Kaka & Bebe

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

struct Edge{
    int to;
    int Kaka, Bebe;
};

const int INF(INT_MAX/2);

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int N, M; cin>>N>>M;
    vector<vector<Edge>> graph(N);
    for(int i(0); i<=M-1; ++i){
        int a, b, c, d; cin>>a>>b>>c>>d;
        if(c>=1001 || d>=1001){
            continue;
        }
        graph[a].push_back(Edge{b, c, d});
        graph[b].push_back(Edge{a, c, d});
    }
    vector<vector<int>> dist(N, vector<int>(1001, INF));
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<tuple<int, int, int>>> pq;
    dist[0][0]=0;
    pq.push({0, 0, 0});
    while(!pq.empty()){
        tuple<int, int, int> cur(pq.top()); pq.pop();
        if(dist[get<1>(cur)][get<2>(cur)]<get<0>(cur)){
            continue;
        }
        for(Edge &edge: graph[get<1>(cur)]){
            tuple<int, int, int> nxt({get<0>(cur)+edge.Kaka, edge.to, get<2>(cur)+edge.Bebe});
            if(get<0>(nxt)>=1001 || get<2>(nxt)>=1001){
                continue;
            }
            if(dist[get<1>(nxt)][get<2>(nxt)]>get<0>(nxt)){
                dist[get<1>(nxt)][get<2>(nxt)]=get<0>(nxt);
                pq.push(nxt);
            }
        }
    }
    int ans(INF);
    for(int i(0); i<=1000; ++i){
        if(dist[N-1][i]<=1000){
            ans=min(ans, i*dist[N-1][i]);
        }
    }
    cout<<(ans==INF?-1:ans);

    return 0;
}