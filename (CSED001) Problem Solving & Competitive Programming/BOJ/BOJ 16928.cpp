// BOJ 16928: 뱀과 사다리 게임

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
// const vector<pair<int, int>> delta{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}, UDLR{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

map<int, int> portal;
vector<int> vst(101, -1);

void BFS(){
    deque<int> q;
    q.push_back(1);
    vst[1]=0;
    while(!q.empty()){
        int cur(q.front()); q.pop_front();
        for(int i(1); i<=6; ++i){
            int nxt(cur+i);
            if(nxt>100){
                continue;
            }
            while(portal.find(nxt)!=portal.end()){
                nxt=portal[nxt];
            }
            if(vst[nxt]==-1){
                q.push_back(nxt);
                vst[nxt]=vst[cur]+1;
            }
        }
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int N, M; cin>>N>>M;
    for(int i(0); i<=N-1; ++i){
        int x, y; cin>>x>>y;
        portal[x]=y;
    }
    for(int i(0); i<=M-1; ++i){
        int u, v; cin>>u>>v;
        portal[u]=v;
    }
    BFS();
    cout<<vst[100];

    return 0;
}