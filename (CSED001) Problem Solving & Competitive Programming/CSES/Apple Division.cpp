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

int n;
vector<ll> p;
ll ans(LLONG_MAX);

void division(int cur, ll left, ll right){
    if(cur==n){
        ans=min(ans, abs(left-right));
        return;
    }
    division(cur+1, left+p[cur], right);
    division(cur+1, left, right+p[cur]);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    cin>>n;
    p.resize(n); for(int i(0); i<=n-1; ++i) cin>>p[i];
    division(0, 0LL, 0LL);
    cout<<ans;

    return 0;
}