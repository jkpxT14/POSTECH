// JUNGOL 1695: 단지번호붙이기

#include <bits/stdc++.h>

using namespace std;

using ll=long long;
using ull=unsigned long long;

#define elif else if

const auto strnpos(string::npos);

template <typename TN1, typename TN2>
pair<TN1, TN2> operator+(const pair<TN1, TN2> &x, const pair<TN1, TN2> &y){
    return {x.first+y.first, x.second+y.second};
}

template <typename TN1, typename TN2>
pair<TN1, TN2> &operator+=(pair<TN1, TN2> &x, const pair<TN1, TN2> &y){
    x.first+=y.first; x.second+=y.second;
    return x;
}

const array<pair<int, int>, 4> delta{{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};

int N;
vector<vector<int>> board;
vector<vector<bool>> vst;
int cnt(0);

void DFS(pair<int, int> cur){
    vst[cur.first][cur.second]=true;
    ++cnt;
    for(pair<int, int> dt: delta){
        pair<int, int> nxt(cur+dt);
        if(nxt.first<=-1 || nxt.first>=N || nxt.second<=-1 || nxt.second>=N){
            continue;
        }
        if(vst[nxt.first][nxt.second] || board[nxt.first][nxt.second]==0){
            continue;
        }
        DFS(nxt);
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    cin>>N;
    board.resize(N, vector<int>(N));
    vst.resize(N, vector<bool>(N, false));
    for(int i(0); i<=N-1; ++i){
        string s; cin>>s;
        for(int j(0); j<=N-1; ++j){
            board[i][j]=s[j]-'0';
        }
    }
    vector<int> ans;
    for(int i(0); i<=N-1; ++i){
        for(int j(0); j<=N-1; ++j){
            if(!vst[i][j] && board[i][j]==1){
                DFS(pair<int, int>{i, j});
                ans.push_back(cnt);
                cnt=0;
            }
        }
    }
    sort(ans.begin(), ans.end());
    cout<<ans.size()<<'\n';
    for(int a: ans){
        cout<<a<<'\n';
    }

    return 0;
}