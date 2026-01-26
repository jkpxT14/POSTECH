// BOJ 7576: 토마토

#include <bits/stdc++.h>

using namespace std;

using ll=long long;

using pqig=priority_queue<int, vector<int>, greater<int>>;
using pqllg=priority_queue<long long, vector<long long>, greater<long long>>;
using mii=map<int, int>;
using umii=unordered_map<int, int>;

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
const vector<pair<int, int>> delta{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

int M, N;
vector<vector<int>> tmt;
deque<pair<int, int>> q;

void BFS(){
    while(!q.empty()){
        pair<int, int> cur(q.front()); q.pop_front();
        for(pair<int, int> d: delta){
            pair<int, int> nxt(cur+d);
            if(0<=nxt.first && nxt.first<=M-1 && 0<=nxt.second && nxt.second<=N-1 && tmt[nxt.first][nxt.second]==0){
                tmt[nxt.first][nxt.second]=tmt[cur.first][cur.second]+1;
                q.push_back(nxt);
            }
        }
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    cin>>M>>N;
    tmt.resize(M, vector<int>(N));
    for(int j(0); j<=N-1; ++j){
        for(int i(0); i<=M-1; ++i){
            cin>>tmt[i][j];
            if(tmt[i][j]==1){
                q.push_back({i, j});
            }
        }
    }
    BFS();
    int ans(-1);
    for(int i(0); i<=M-1; ++i){
        for(int j(0); j<=N-1; ++j){
            if(!tmt[i][j]){
                cout<<-1;
                return 0;
            }
            ans=max(ans, tmt[i][j]-1);
        }
    }
    cout<<ans;
    return 0;
}