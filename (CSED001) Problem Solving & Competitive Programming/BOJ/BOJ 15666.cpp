// BOJ 15666: N과 M (12)

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
vector<int> num;

void DFS(vector<int> sq, int last_term_idx){
    if(sq.size()==M){
        for(int i(0); i<=M-1; ++i){
            cout<<sq[i]<<" \n"[i==M-1];
        }
        return;
    }
    int duplication(0);
    for(int i(last_term_idx); i<=N; ++i){
        if(num[i]!=duplication){
            sq.push_back(num[i]);
            DFS(sq, i);
            sq.pop_back();
            duplication=num[i];
        }
    }
    return;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    cin>>N>>M;
    num.resize(N+1);
    for(int i(1); i<=N; ++i) cin>>num[i];
    sort(num.begin()+1, num.end());
    DFS({}, 1);

    return 0;
}