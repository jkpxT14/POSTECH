// BOJ 31785: 시소 배열

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
pair<TN1, TN2> &operator+=(pair<TN1, TN2> &x, const pair<TN1, TN2> &y){
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

    int Q; cin>>Q;
    deque<int> A;
    while(Q--){
        int type; cin>>type;
        if(type==1){
            int x; cin>>x;
            A.push_back(x);
        } elif(type==2){
            int L(int(A.size())), M(L/2), N(L-M);
            pair<int, int> sum; sum.first=0; sum.second=0;
            for(int i(0); i<=M-1; ++i){
                sum.first+=A[i];
            }
            for(int i(M); i<=L-1; ++i){
                sum.second+=A[i];
            }
            if(sum.first<=sum.second){
                for(int i(0); i<=M-1; ++i){
                    A.pop_front();
                }
                cout<<sum.first<<'\n';
            } elif(sum.first>sum.second){
                for(int i(0); i<=N-1; ++i){
                    A.pop_back();
                }
                cout<<sum.second<<'\n';
            }
        }
    }
    for(int i(0); i<=int(A.size())-1; ++i){
        cout<<A[i]<<' ';
    }

    return 0;
}