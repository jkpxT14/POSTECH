// Baekjoon Online Judge Prob. 1197

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <tuple>
#include <limits>

using namespace std;

using ll=long long;
using vi=vector<int>;
using vb=vector<bool>;
using pii=pair<int, int>;
using vpii=vector<pair<int, int>>;
using vvpii=vector<vector<pair<int, int>>>;
#define INT_MAX 2147483647

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    // cout.tie(nullptr);

    int V, E, ans_weight=0;
    // vvpii adj(10001, vpii(10001));
    vb visited(10001);
    priority_queue<pii, vpii, greater<pii>> pq;
    cin>>V>>E;
    vvpii adj(V+1);
    for(int i=1; i<=V; i++){
        visited[i]=false;
    }
    for(int i=0; i<E; i++){
        int A, B, C;
        cin>>A>>B>>C;
        adj[A].push_back(make_pair(C, B));
        adj[B].push_back(make_pair(C, A));
    }
    pq.push(make_pair(0, 1));
    while(!pq.empty()){
        int cur_weight=pq.top().first;
        int cur_vertex=pq.top().second;
        pq.pop();
        if(visited[cur_vertex]) continue;
        ans_weight+=cur_weight;
        visited[cur_vertex]=true;
        for(auto &edge: adj[cur_vertex]){
            if(!visited[edge.second]) pq.push(edge);
        }
    }
    cout<<ans_weight;
    return 0;
}