// BOJ 14500: 테트로미노

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
const array<pair<int, int>, 4> delta{{{1, 0}, {-1, 0}, {0, 1}, {0, -1}}};

int N, M;
vector<vector<int>> paper;
vector<vector<bool>> vst;
int ans(0);

void DFS(pair<int, int> cur, int cnt, int sum){
    if(cnt==4){
        ans=max(ans, sum);
        return;
    }
    for(pair<int, int> d: delta){
        pair<int, int> nxt(cur+d);
        if(nxt.first<=-1 || nxt.first>=N || nxt.second<=-1 || nxt.second>=M || vst[nxt.first][nxt.second]){
            continue;
        }
        vst[nxt.first][nxt.second]=true;
        DFS(nxt, cnt+1, sum+paper[nxt.first][nxt.second]);
        vst[nxt.first][nxt.second]=false;
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    cin>>N>>M;
    paper.resize(N, vector<int>(M));
    for(int i(0); i<=N-1; ++i){
        for(int j(0); j<=M-1; ++j){
            cin>>paper[i][j];
        }
    }
    for(int i(0); i<=N-1; ++i){
        for(int j(0); j<=M-1; ++j){
            if(i+1<=N-1 && j+1<=M-1 && j-1>=0){
                ans=max(ans, paper[i][j]+paper[i+1][j]+paper[i][j+1]+paper[i][j-1]);
            }
            if(i-1>=0 && j-1>=0 && j+1<=M-1){
                ans=max(ans, paper[i][j]+paper[i-1][j]+paper[i][j-1]+paper[i][j+1]);
            }
            if(j+1<=M-1 && i-1>=0 && i+1<=N-1){
                ans=max(ans, paper[i][j]+paper[i][j+1]+paper[i-1][j]+paper[i+1][j]);
            }
            if(j-1>=0 && i+1<=N-1 && i-1>=0){
                ans=max(ans, paper[i][j]+paper[i][j-1]+paper[i+1][j]+paper[i-1][j]);
            }
        }
    }
    vst.assign(N, vector<bool>(M, false));
    for(int i(0); i<=N-1; ++i){
        for(int j(0); j<=M-1; ++j){
            vst[i][j]=true;
            DFS({i, j}, 1, paper[i][j]);
            vst[i][j]=false;
        }
    }
    cout<<ans;

    return 0;
}