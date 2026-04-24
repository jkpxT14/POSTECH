// BOJ 25552: 잔디 예측하기

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

// const ll mdls(1000000007LL);
// const int INF(INT_MAX/4);
// const int offset(500000);
// const array<pair<int, int>, 4> delta{{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int N, M; cin>>N>>M;
    vector<vector<char>> present(N, vector<char>(M));
    for(int i(0); i<=N-1; ++i){
        for(int j(0); j<=M-1; ++j){
            cin>>present[i][j];
        }
    }
    int D; cin>>D;
    vector<vector<char>> future(N, vector<char>(M));
    for(int i(0); i<=N-1; ++i){
        for(int j(0); j<=M-1; ++j){
            cin>>future[i][j];
        }
    }
    vector<pair<int, int>> within;
    for(int dx(-D); dx<=D; ++dx){
        for(int dy(-D); dy<=D; ++dy){
            if(abs(dx)+abs(dy)<=D){
                within.push_back({dx, dy});
            }
        }
    }
    deque<pair<int, int>> q;
    vector<vector<bool>> vst(N, vector<bool>(M, false));
    for(int i(0); i<=N-1; ++i){
        for(int j(0); j<=M-1; ++j){
            if(present[i][j]=='O' && future[i][j]=='X'){
                cout<<"NO";
                return 0;
            }
            if(present[i][j]=='O'){
                q.push_back({i, j});
                vst[i][j]=true;
            }
        }
    }
    while(!q.empty()){
        pair<int, int> cur(q.front()); q.pop_front();
        for(pair<int, int> d: within){
            pair<int, int> nxt(cur+d);
            if(nxt.first<0 || nxt.first>=N || nxt.second<0 || nxt.second>=M){
                continue;
            }
            if(vst[nxt.first][nxt.second]){
                continue;
            }
            if(future[nxt.first][nxt.second]=='X'){
                continue;
            }
            q.push_back(nxt);
            vst[nxt.first][nxt.second]=true;
        }
    }
    for(int i(0); i<=N-1; ++i){
        for(int j(0); j<=M-1; ++j){
            if(future[i][j]=='O' && !vst[i][j]){
                cout<<"NO";
                return 0;
            }
        }
    }
    cout<<"YES";
    return 0;
}