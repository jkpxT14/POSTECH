// BOJ 17241: Pineapple Advertising

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
pair<TN1, TN2> & operator+=(pair<TN1, TN2> &x, const pair<TN1, TN2> &y){
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

    int N, M, Q; cin>>N>>M>>Q;
    vector<vector<int>> adj(N+1);
    for(int i(1); i<=M; ++i){
        int a, b; cin>>a>>b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    vector<bool> vst1(N+1, false), vst2(N+1, false);
    for(int i(1); i<=Q; ++i){
        int n; cin>>n;
        int cnt(0);
        if(vst1[n]){
            cout<<cnt<<'\n';
            continue;
        } elif(!vst1[n] && vst2[n]){
            vst1[n]=true;
        } elif(!vst2[n]){
            ++cnt;
            vst1[n]=true; vst2[n]=true;
        }
        for(int connected: adj[n]){
            if(!vst2[connected]){
                ++cnt;
                vst2[connected]=true;
            }
        }
        cout<<cnt<<'\n';
    }

    return 0;
}