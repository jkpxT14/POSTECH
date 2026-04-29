// JUNGOL 3335: 양팔저울

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

int k;
vector<int> g;
int S(0);
vector<bool> measureable;

void f(int sum, int idx){
    measureable[sum+S]=true;
    if(idx==k){
        return;
    }
    f(sum, idx+1);
    f(sum+g[idx], idx+1);
    f(sum-g[idx], idx+1);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    cin>>k;
    g.resize(k);
    for(int i(0); i<=k-1; i++){
        cin>>g[i];
        S+=g[i];
    }
    measureable.resize(2*S+1, false);
    f(0, 0);
    int cnt(0);
    for(int i(S+1); i<=2*S; ++i){
        if(!measureable[i]){
            ++cnt;
        }
    }
    cout<<cnt;

    return 0;
}