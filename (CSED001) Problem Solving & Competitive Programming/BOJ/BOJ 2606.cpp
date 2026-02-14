// BOJ 2606: 바이러스

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

vector<vector<int>> computer;
vector<bool> vst;
int ans(0);

void DFS(int cur){
    vst[cur]=true;
    ++ans;
    for(int nxt: computer[cur]){
        if(!vst[nxt]){
            DFS(nxt);
        }
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int N; cin>>N;
    computer.resize(N+1);
    vst.resize(N+1, false);
    int M; cin>>M;
    for(int i(1); i<=M; ++i){
        int a, b; cin>>a>>b;
        computer[a].push_back(b);
        computer[b].push_back(a);
    }
    DFS(1);
    cout<<ans-1;

    return 0;
}