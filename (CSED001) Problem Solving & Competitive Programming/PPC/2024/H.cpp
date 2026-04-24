// BOJ 31782: 저체온증

#include <bits/stdc++.h>

using namespace std;

using ll=long long;
using ull=unsigned long long;

#define elif else if

const auto strnpos(string::npos);

template <typename TN1, typename TN2>
pair<TN1, TN2> operator +(const pair<TN1, TN2> &x, const pair<TN1, TN2> &y){
    return {x.first+y.first, x.second+y.second};
}

template <typename TN1, typename TN2>
pair<TN1, TN2> &operator +=(pair<TN1, TN2> &x, const pair<TN1, TN2> &y){
    x.first+=y.first; x.second+=y.second;
    return x;
}

const array<pair<int, int>, 4> delta{{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int N, M, K; cin>>N>>M>>K;
    vector<vector<char>> board(N, vector<char>(M));
    deque<pair<int, int>> q;
    vector<vector<int>> cnt(N, vector<int>(M, 0));
    for(int i(0); i<=N-1; ++i){
        for(int j(0); j<=M-1; ++j){
            cin>>board[i][j];
            if(board[i][j]=='O'){
                q.push_back({i, j});
            }
        }
    }
    while(!q.empty()){
        pair<int, int> cur(q.front()); q.pop_front();
        for(pair<int, int> d: delta){
            pair<int, int> nxt(cur+d);
            if(nxt.first<0 || nxt.first>=N || nxt.second<0 || nxt.second>=M){
                continue;
            }
            if(board[nxt.first][nxt.second]=='O'){
                continue;
            }
            ++cnt[nxt.first][nxt.second];
            if(cnt[nxt.first][nxt.second]==2){
                board[nxt.first][nxt.second]='O';
                q.push_back(nxt);
            }
        }
    }
    int ans(0);
    for(int i(0); i<=N-1; ++i){
        for(int j(0); j<=M-1; ++j){
            if(board[i][j]!='O' || (i>=1 && board[i-1][j]=='O') || (j>=1 && board[i][j-1]=='O')){
                continue;
            }
            int h(0), w(0);
            while(i+h<=N-1 && board[i+h][j]=='O') ++h;
            while(j+w<=M-1 && board[i][j+w]=='O') ++w;
            if(h>K && w>K) ans+=h*w;
        }
    }
    cout<<ans;

    return 0;
}