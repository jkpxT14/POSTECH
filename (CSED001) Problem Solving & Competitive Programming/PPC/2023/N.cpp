// BOJ 28110: 마지막 문제

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
    vector<int> A(N+1); for(int i(1); i<=N; ++i) cin>>A[i];
    sort(A.begin()+1, A.end());
    int max_diff(INT_MIN), max_diff_index(-1);
    for(int i(2); i<=N; ++i){
        if((max_diff/2)<((A[i]-A[i-1])/2)){
            max_diff=A[i]-A[i-1];
            max_diff_index=i;
        }
    }
    if(max_diff==1){
        cout<<-1;
    } else{
        cout<<A[max_diff_index-1]+(max_diff/2);
    }

    return 0;
}