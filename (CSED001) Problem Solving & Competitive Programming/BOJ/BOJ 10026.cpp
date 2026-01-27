// BOJ 10026: Cow Art

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

int N;
vector<vector<char>> painting;
vector<vector<bool>> vst;

void DFS(pair<int, int> cur){
    vst[cur.first][cur.second]=true;
    for(pair<int, int> d: delta){
        pair<int, int> nxt(cur+d);
        if(0<=nxt.first && nxt.first<=N-1 && 0<=nxt.second && nxt.second<=N-1 && painting[cur.first][cur.second]==painting[nxt.first][nxt.second] && !vst[nxt.first][nxt.second]){
            DFS(nxt);
        }
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    cin>>N;
    painting.resize(N, vector<char>(N));
    for(int i(0); i<=N-1; ++i){
        for(int j(0); j<=N-1; ++j){
            cin>>painting[i][j];
        }
    }
    vst.assign(N, vector<bool>(N, false));
    pair<int, int> ans{0, 0};
    for(int i(0); i<=N-1; ++i){
        for(int j(0); j<=N-1; ++j){
            if(!vst[i][j]){
                DFS({i, j});
                ++ans.first;
            }
        }
    }
    for(int i(0); i<=N-1; ++i){
        for(int j(0); j<=N-1; ++j){
            if(painting[i][j]=='G'){
                painting[i][j]='R';
            }
        }
    }
    vst.assign(N, vector<bool>(N, false));
    for(int i(0); i<=N-1; ++i){
        for(int j(0); j<=N-1; ++j){
            if(!vst[i][j]){
                DFS({i, j});
                ++ans.second;
            }
        }
    }
    cout<<ans.first<<' '<<ans.second;

    return 0;
}