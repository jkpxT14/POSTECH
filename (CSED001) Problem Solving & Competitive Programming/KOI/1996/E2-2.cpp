// JUNGOL 1459: 숫자고르기(Finding Cycles in a Mapping)

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

// const ll mdls(1000000007LL);
// const int INF(INT_MAX/4);
// const int offset(500000);
// const array<pair<int, int>, 4> delta{{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};

int N;
vector<int> a;
set<int> ans;

void DFS(int src, int cur, vector<bool> &vst, vector<int> &path){
    if(vst[cur]){
        if(cur==src){
            for(int x: path){
                ans.insert(x);
            }
        }
        return;
    }
    vst[cur]=true;
    path.push_back(cur);
    DFS(src, a[cur], vst, path);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    cin>>N;
    a.resize(N+1); for(int i(1); i<=N; ++i) cin>>a[i];
    for(int i(1); i<=N; ++i){
        vector<bool> vst(N+1, false);
        vector<int> path;
        DFS(i, i, vst, path);
    }
    cout<<ans.size()<<'\n';
    for(int x: ans){
        cout<<x<<'\n';
    }

    return 0;
}