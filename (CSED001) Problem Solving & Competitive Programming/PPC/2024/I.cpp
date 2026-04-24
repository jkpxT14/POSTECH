// BOJ 31783: 78계단 내려가기 대회

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

    int N; cin>>N;
    vector<int> H(N+1); for(int i(1); i<=N; ++i) cin>>H[i];
    vector<int> A(N+1); for(int i(2); i<=N; ++i) cin>>A[i];
    vector<int> B(N+1); for(int i(2); i<=N; ++i) cin>>B[i];
    vector<ll> DP(N+1, 0);
    for(int i(2); i<=N; ++i){
        int lo(1), hi(i-1), f(0);
        while(lo<=hi){
            int mid((lo+hi)>>1);
            if(H[mid]>=H[i]+B[i]){
                f=mid;
                lo=mid+1;
            } else{
                hi=mid-1;
            }
        }
        DP[i]=DP[i-1];
        if(f!=0){
            DP[i]=max(DP[i], DP[f]+ll(A[i]));
        }
    }
    cout<<DP[N];

    return 0;
}