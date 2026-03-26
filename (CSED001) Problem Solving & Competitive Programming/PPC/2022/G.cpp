// BOJ 25550: 택배 색칠

#include <bits/stdc++.h>

using namespace std;

using ll=long long;
using ull=unsigned long long;

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

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int N, M; cin>>N>>M;
    vector<vector<ll>> A(N+1, vector<ll>(M+1));
    for(int i(1); i<=N; ++i){
        for(int j(1); j<=M; ++j){
            cin>>A[i][j];
        }
    }
    ll ans(0);
    for(int i(2); i<=N-1; ++i){
        for(int j(2); j<=M-1; ++j){
            ll C(A[i][j]), U(A[i-1][j]), D(A[i+1][j]), L(A[i][j-1]), R(A[i][j+1]);
            ll B(min(U, min(D, min(L, R))));
            if(!C){
                continue;
            } elif(C<=B){
                ans+=C-1;
            } else{
                ans+=B;
            }
        }
    }
    cout<<ans;

    return 0;
}