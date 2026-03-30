// BOJ 35466: 대회 장소 정하기

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

    int N; cin>>N;
    int a, b, c; cin>>a>>b>>c;
    int p, q; cin>>p>>q;

    auto dist = [N](int x, int y) -> int {
        if(x>y){
            swap(x, y);
        }
        return min(y-x, N-(y-x));
    };

    int U(0), D(a), P((dist(D+b, 0)==c)?(D+b):((D-b+N)%N));
    for(int i(0); i<=N-1; ++i){
        if(dist(i, U)==p && dist(i, D)==q){
            cout<<dist(i, P);
            return 0;
        }
    }
}