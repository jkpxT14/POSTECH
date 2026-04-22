// BOJ 23816: 옷걸이걸이걸이

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

    int N, M; cin>>N>>M;
    int ans(-1);
    for(int d(0); d<=N && 15*d<=M; ++d){
        for(int c(0); c+d<=N && 7*c+15*d<=M; ++c){
            int P(N-c-d), Q(M-7*c-15*d);
            int b(max(0, int(ceil((double(Q)-double(P))/2.0))));
            int a(Q-3*b);
            if(a>=0){
                ans=max(ans, a+2*b+4*c);
            }
        }
    }
    cout<<ans;

    return 0;
}