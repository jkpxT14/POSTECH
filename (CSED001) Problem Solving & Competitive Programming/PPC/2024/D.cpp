// BOJ 31778: PPC 만들기

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

    int N, K; cin>>N>>K;
    string S; cin>>S;
    int l(0), r(N-1);
    while(K--){
        while(l<=N-1 && S[l]!='C') ++l;
        while(r>=0 && S[r]!='P') --r;
        if(l>=r) break;
        swap(S[l], S[r]);
        ++l; --r;
    }
    int p(0);
    ll ans(0);
    for(char c: S){
        if(c=='P') ++p;
        elif(c=='C') ans+=ll(p)*(ll(p)-1LL)/2LL;
    }
    cout<<ans;

    return 0;
}