// BOJ 2150: Strongly Connected Component

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
// const int INF(INT_MAX/4);
// const int offset(500000);
// const array<pair<int, int>, 4> delta{{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};

int V, E;
vector<vector<int>> graph;
vector<int> dsc, low;
int timer(0);
deque<int> stk;
vector<bool> in_stk;
vector<vector<int>> SCC;

void DFS(int cur){
    dsc[cur]=low[cur]=++timer;
    stk.push_back(cur);
    in_stk[cur]=true;
    for(int nxt: graph[cur]){
        if(!dsc[nxt]){
            DFS(nxt);
            low[cur]=min(low[cur], low[nxt]);
        } elif(in_stk[nxt]){
            low[cur]=min(low[cur], dsc[nxt]);
        }
    }
    if(low[cur]==dsc[cur]){
        vector<int> scc;
        while(true){
            int v(stk.back()); stk.pop_back();
            in_stk[v]=false;
            scc.push_back(v);
            if(v==cur){
                break;
            }
        }
        sort(scc.begin(), scc.end());
        SCC.push_back(scc);
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    cin>>V>>E;
    graph.resize(V+1);
    dsc.resize(V+1, 0); low.resize(V+1, 0);
    in_stk.resize(V+1, false);
    for(int i(1); i<=E; ++i){
        int from, to; cin>>from>>to;
        graph[from].push_back(to);
    }
    for(int i(1); i<=V; ++i){
        if(!dsc[i]){
            DFS(i);
        }
    }
    sort(SCC.begin(), SCC.end());
    cout<<SCC.size()<<'\n';
    for(vector<int> scc: SCC){
        for(int v: scc){
            cout<<v<<' ';
        }
        cout<<"-1\n";
    }

    return 0;
}