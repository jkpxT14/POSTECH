// Reference: BOJ 1260: DFS와 BFS

#include <bits/stdc++.h>

using namespace std;

using ll=long long;
using pii=pair<int, int>;
using pll=pair<long long, long long>;
using pci=pair<char, int>;
using pipii=pair<int, pair<int, int>>;
using ppiii=pair<pair<int, int>, int>;
using vi=vector<int>;
using vll=vector<long long>;
using vd=vector<double>;
using vb=vector<bool>;
using vc=vector<char>;
using vstr=vector<string>;
using vpii=vector<pair<int, int>>;
using vpll=vector<pair<long long, long long>>;
using vpci=vector<pair<char, int>>;
using vpipii=vector<pair<int, pair<int, int>>>;
using vppiii=vector<pair<pair<int, int>, int>>;
using vvi=vector<vector<int>>;
using vvll=vector<vector<long long>>;
using vvc=vector<vector<char>>;
using vvpii=vector<vector<pair<int, int>>>;
using vvvi=vector<vector<vector<int>>>;
using vvvll=vector<vector<vector<long long>>>;
using dqi=deque<int>;
using dqll=deque<long long>;
using dqpii=deque<pair<int, int>>;
using pqi=priority_queue<int>;
using pqig=priority_queue<int, vector<int>, greater<int>>;
using pqll=priority_queue<long long>;
using pqllg=priority_queue<long long, vector<long long>, greater<long long>>;
using mii=map<int, int>;

#define elif else if
const auto strnpos=string::npos;

// const int INF(INT_MAX);
// const ll mod(1000000007); // 10^9+7
// const int offset(500000);

int N, M, V;
vvi adj;
vb visited;

void init(){
    cin>>N>>M>>V;
    adj.resize(N+1, vi(N+1, 0));
    visited.resize(N+1, false);

    for(int i=0; i<M; i++){
        int A, B;
        cin>>A>>B;
        adj[A][B]=1;
        adj[B][A]=1;
    }
}

void DFS(){
    stack<int> sta;
    bool deeper=false;

    sta.push(V);
    visited[V]=true;
    cout<<V<<" ";
    while(!sta.empty()){
        for(int i=1; i<=N; i++){
            if(adj[sta.top()][i]==1 && !visited[i]){
                sta.push(i);
                visited[i]=true;
                cout<<i<<" ";
                deeper=true;
                break;
            }
        }
        if(deeper) deeper=false;
        else sta.pop();
    }
}

void BFS(){
    queue<int> que;
    int cur_node;

    que.push(V);
    visited[V]=true;
    cout<<V<<" ";
    while(!que.empty()){
        cur_node=que.front();
        que.pop();
        for(int i=1; i<=N; i++){
            if(adj[cur_node][i]==1 && !visited[i]){
                que.push(i);
                visited[i]=true;
                cout<<i<<" ";
            }
        }
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed;
    cout.precision(10);

    init();
    DFS();
    cout<<"\n";
    visited.assign(N+1, false);
    BFS();
    
    return 0;
}