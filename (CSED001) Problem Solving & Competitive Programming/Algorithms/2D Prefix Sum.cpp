// Last Update on 20260220

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

    int M, N; cin>>M>>N;
    vector<vector<ll>> A(M+1, vector<ll>(N+1));
    for(int i(1); i<=M; ++i){
        for(int j(1); j<=N; ++j){
            cin>>A[i][j];
        }
    }
    vector<vector<ll>> P(M+1, vector<ll>(N+1, 0));
    for(int i(1); i<=M; ++i){
        for(int j(1); j<=N; ++j){
            P[i][j]=A[i][j]+P[i][j-1]+P[i-1][j]-P[i-1][j-1];
        }
    }
    for(int i(1); i<=M; ++i){
        for(int j(1); j<=N; ++j){
            cout<<P[i][j]<<" \n"[j==N];
        }
    }

    return 0;
}