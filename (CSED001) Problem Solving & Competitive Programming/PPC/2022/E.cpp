// BOJ 25547: 신기한 숫자

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

    int A, B; cin>>A>>B;
    if(B%A!=0){
        cout<<0;
        return 0;
    }
    int D(B/A);
    int ans(1);
    for(int p(2); p*p<=D; ++p){
        if(D%p){
            continue;
        }
        int cnt(0);
        while(!(D%p)){
            D/=p;
            ++cnt;
        }
        ans*=(cnt+1);
    }
    if(D>1){
        ans*=2;
    }
    cout<<ans;
    return 0;
}