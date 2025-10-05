#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

using ll=long long;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    const int INF=100000; // [Caution] const int INF=2147483657
    int V, E, K;
    // vector<vector<int>> weight(20001, vector<int> (20001, INF));
    // int weight[20001][20001]; // MLE?
    int weight[201][201];
    // vector<int> dist(20001, INF);
    int dist[201];
    // [Need to Study] vector<bool> visit(20001, false);
    bool visit[201];
    cin>>V>>E;
    cin>>K;
    for(int i=1; i<=V; i++){
        for(int j=1; j<=V; j++){
            weight[i][j]=INF;
        }
    }
    for(int i=1; i<=V; i++){
        weight[i][i]=0;
        dist[i]=INF;
        visit[i]=false;
    }
    for(int i=1; i<=E; i++){
        int u, v, w;
        cin>>u>>v>>w;
        weight[u][v]=w;
        // weight[v][u]=w;
    }
    // for(int i=1; i<=V; i++){
    //     for(int j=1; j<=V; j++){
    //         cout<<weight[i][j]<<" ";
    //     }
    //     cout<<"\n";
    // }

    for(int i=1; i<=V; i++){
        dist[i]=weight[K][i];
    }
    visit[K]=true;
    for(int i=1; i<=V; i++){
        int current_vertex=-1, min_value=INF, min_index=-1;
        for(int j=1; j<=V; j++){
            if(min_value>dist[j] && !visit[j]){
                min_value=dist[j];
                min_index=j;
            }
        }
        current_vertex=min_index;
        if(current_vertex==-1){
            continue;
        }
        for(int j=1; j<=V; j++){
            if(!visit[j]){
                dist[j]=min(dist[j], dist[current_vertex]+weight[current_vertex][j]);
            }
        }
        visit[current_vertex]=true;
    }

    for(int i=1; i<=V; i++){
        if(dist[i]==INF){
            cout<<"INF"<<"\n";
            continue;
        }
        cout<<dist[i]<<"\n";
    }
}