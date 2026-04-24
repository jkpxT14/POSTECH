// BOJ 28109: 약속 장소 2

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
    if(K>25*N+1){
        cout<<-1;
        return 0;
    }
    for(int i(0); i<=N-1; ++i){
        int cnt(S[i]-'A');
        if(K<=cnt){
            S[i]='A'+K-1;
            cout<<S;
            return 0;
        }
        K-=cnt;
    }
    if(K==1){
        cout<<S;
        return 0;
    }
    --K;
    for(int i(N-1); i>=0; --i){
        int cnt('Z'-S[i]);
        if(K<=cnt){
            S[i]=S[i]+K;
            cout<<S;
            return 0;
        }
        K-=cnt;
    }
    return 0;
}