// BOJ 28106: 나무 타기

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

const int mdls(998244353);

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int N; cin>>N;
    vector<int> P(N+1);
    vector<vector<int>> C(N+1);
    int root(0);
    for(int i(1); i<=N; ++i){
        cin>>P[i];
        if(P[i]==0){
            root=i;
        } else{
            C[P[i]].push_back(i);
        }
    }
    vector<int> A(N+1); for(int i(1); i<=N; ++i) cin>>A[i];
    vector<int> D(N+1);
    vector<int> order;
    function<void(int)> DFS=[&](int cur){
        order.push_back(cur);
        for(int nxt: C[cur]){
            D[nxt]=D[cur]+1;
            DFS(nxt);
        }
    };
    DFS(root);
    vector<int> DP(N+1, 0);
    DP[root]=1;
    for(int nd: order){
        if(nd==root){
            continue;
        }
        for(int p(P[nd]); p!=0; p=P[p]){
            if(D[nd]-D[p]<=A[p]){
                DP[nd]+=(DP[p]%mdls);
                DP[nd]%=mdls;
            }
        }
    }
    int ans(0);
    for(int i(1); i<=N; ++i){
        if(C[i].empty()){
            ans+=DP[i];
            ans%=mdls;
        }
    }
    cout<<ans;

    return 0;
}