#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>

using namespace std;

using ll=long long;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    const int INF=1000000; // [Caution] const int INF=2147483657 or 10000 or 100000
    int V, E, K;
    vector<pair<int, int>> graph[20001];
    vector<int> dist(20001, INF);
    priority_queue<pair<int, int>> pq;
    cin>>V>>E;
    cin>>K;
    for(int i=1; i<=E; i++){
        int u, v, w;
        cin>>u>>v>>w;
        graph[u].push_back(make_pair(v, w));
        // graph[v].push_back(make_pair(u, w));
    }

    dist[K]=0;
    pq.push(make_pair(0, K));
    while(!pq.empty()){
        int current_dist=-pq.top().first;
        int current_vertex=pq.top().second;
        pq.pop();
        for(int i=0; i<graph[current_vertex].size(); i++){
            int next_vertex=graph[current_vertex][i].first; // [Caution] int next_vertex=graph[current_vertex][i].first()
            int next_dist=current_dist+graph[current_vertex][i].second;
            if(dist[next_vertex]>next_dist){
                dist[next_vertex]=next_dist;
                pq.push(make_pair(-next_dist, next_vertex));
            }
        }
    }

    for(int i=1; i<=V; i++){
        if(dist[i]==INF){
            cout<<"INF"<<"\n";
            continue;
        }
        cout<<dist[i]<<"\n";
    }
}