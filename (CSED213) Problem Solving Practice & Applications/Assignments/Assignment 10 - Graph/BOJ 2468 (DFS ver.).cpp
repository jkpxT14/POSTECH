// BOJ 2468: 안전 영역

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
// const ll mod(1000000007); // 10^9+7
// const int offset(500000);
const vpii delta{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

int N;
vvb safe, visited;

void dfs(int x, int y){
    visited[x][y]=true;
    for(int i(0); i<4; ++i){
        int nx(x+delta[i].first), ny(y+delta[i].second);
        if(0<=nx && nx<N && 0<=ny && ny<N && safe[nx][ny] && !visited[nx][ny]){
            dfs(nx, ny);
        }
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed;
    cout.precision(10);

    cin>>N;
    vvi height(N, vi(N));
    int h_min(101), h_Max(0);
    for(int i(0); i<N; ++i){
        for(int j(0); j<N; ++j){
            cin>>height[i][j];
            h_min=min(h_min, height[i][j]);
            h_Max=max(h_Max, height[i][j]);
        }
    }
    safe.resize(N, vb(N));
    visited.resize(N, vb(N));
    int ans(1);
    for(int r(h_min); r<=h_Max; ++r){
        safe.assign(N, vb(N));
        visited.assign(N, vb(N, false));
        for(int i(0); i<N; ++i){
            for(int j(0); j<N; ++j){
                safe[i][j]=(height[i][j]>r)?(true):(false);
            }
        }

        // for(int i(0); i<N; ++i){
        //     for(int j(0); j<N; ++j){
        //         cout<<safe[i][j]<<' ';
        //     }
        //     cout<<'\n';
        // }

        int cnt(0);
        for(int i(0); i<N; ++i){
            for(int j(0); j<N; ++j){
                if(safe[i][j] && !visited[i][j]){
                    // cout<<i<<' '<<j<<'\n';

                    dfs(i, j);
                    ++cnt;
                }
            }
        }
        ans=max(ans, cnt);
    }
    cout<<ans;
    
    return 0;
}