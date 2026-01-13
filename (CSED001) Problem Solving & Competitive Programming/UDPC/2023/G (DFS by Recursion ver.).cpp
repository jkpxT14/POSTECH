// BOJ 27924: 윤이는 엄청난 것을 훔쳐갔습니다

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
using vvb=vector<vector<bool>>;
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
// const ll mod(1000000007);
// const int offset(500000);
// const vpii delta{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

int N;
vvi adj;
vi deg;
vi d(3);
vvi dist(3);

void dfs(int id, int prev, int cur){
    for(int next: adj[cur]){
        if(next==prev){
            continue;
        }
        dist[id][next]=dist[id][cur]+1;
        dfs(id, cur, next);
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    cin>>N;
    adj.resize(N+1);
    deg.resize(N+1, 0);
    for(int i(0); i<N-1; ++i){
        int u, v;
        cin>>u>>v;
        adj[u].push_back(v);
        adj[v].push_back(u);
        ++deg[u];
        ++deg[v];
    }
    for(int i(0); i<3; ++i){
        cin>>d[i];
    }
    dist.assign(3, vi(N+1, -1));

    dist[0][d[0]]=0;
    dfs(0, 0, d[0]);
    dist[1][d[1]]=0;
    dfs(1, 0, d[1]);
    dist[2][d[2]]=0;
    dfs(2, 0, d[2]);

    bool escape(false);
    for(int node(1); node<=N; ++node){
        if(deg[node]==1){
            if(dist[0][node]<dist[1][node] && dist[0][node]<dist[2][node]){
                escape=true;
                break;
            }
        }
    }
    cout<<(escape?"YES":"NO");

    return 0;
}