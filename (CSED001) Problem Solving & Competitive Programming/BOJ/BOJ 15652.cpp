// BOJ 15652: N과 M (4)

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

int N, M;

void DFS(vector<int> sq){
    if(sq.size()==M){
        for(int i(0); i<=M-1; ++i){
            cout<<sq[i]<<" \n"[i==M-1];
        }
        return;
    }
    if(sq.empty()){
        for(int i(1); i<=N; ++i){
            sq.push_back(i);
            DFS(sq);
            sq.pop_back();
        }
    } else{
        for(int i(sq.back()); i<=N; ++i){
            sq.push_back(i);
            DFS(sq);
            sq.pop_back();
        }
    }
    return;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    cin>>N>>M;
    DFS({});

    return 0;
}