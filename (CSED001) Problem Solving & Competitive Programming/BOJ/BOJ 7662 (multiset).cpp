// BOJ 7662: Dual Priority Queue

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
// const vector<pair<int, int>> delta{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}, UDLR{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int T; cin>>T;
    while(T--){
        multiset<int> Q;
        int k; cin>>k;
        while(k--){
            char op; int n; cin>>op>>n;
            if(op=='I'){
                Q.insert(n);
            } elif(op=='D'){
                if(Q.empty()){
                    continue;
                }
                if(n==1){
                    Q.erase(prev(Q.end()));
                } elif(n==-1){
                    Q.erase(Q.begin());
                }
            }
        }
        if(!Q.empty()){
            cout<<*prev(Q.end())<<' '<<*Q.begin()<<'\n';
        } else{
            cout<<"EMPTY\n";
        }
    }

    return 0;
}