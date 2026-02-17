// BOJ 1149: RGB거리

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

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int N; cin>>N;
    vector<int> costR(N+1), costG(N+1), costB(N+1); for(int i(1); i<=N; ++i) cin>>costR[i]>>costG[i]>>costB[i];
    vector<int> dpR(N+1), dpG(N+1), dpB(N+1);
    dpR[1]=costR[1];
    dpG[1]=costG[1];
    dpB[1]=costB[1];
    for(int i(2); i<=N; ++i){
        dpR[i]=min(dpG[i-1], dpB[i-1])+costR[i];
        dpG[i]=min(dpR[i-1], dpB[i-1])+costG[i];
        dpB[i]=min(dpR[i-1], dpG[i-1])+costB[i];
    }
    cout<<min(dpR[N], min(dpG[N], dpB[N]));

    return 0;
}