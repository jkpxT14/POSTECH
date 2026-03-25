// BOJ 25551: 멋쟁이 포닉스

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

    int M_W, M_B; cin>>M_W>>M_B;
    int T_W, T_B; cin>>T_W>>T_B;
    int P_W, P_B; cin>>P_W>>P_B;
    int a(min(M_W, min(T_B, P_W))), b(min(M_B, min(T_W, P_B)));
    if(a<b) cout<<2*a+1;
    elif(a==b) cout<<a+b;
    elif(a>b) cout<<2*b+1;

    return 0;
}