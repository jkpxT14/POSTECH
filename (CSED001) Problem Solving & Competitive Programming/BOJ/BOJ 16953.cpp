// BOJ 16953: A -> B

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

    ll A, B; cin>>A>>B;
    deque<pair<ll, int>> q;
    q.push_back({A, 1});
    while(!q.empty()){
        pair<ll, int> cur(q.front()); q.pop_front();
        if(cur.first==B){
            cout<<cur.second;
            return 0;
        }
        if(cur.first*2<=B){
            q.push_back({cur.first*2, cur.second+1});
        }
        if(cur.first*10+1<=B){
            q.push_back({cur.first*10+1, cur.second+1});
        }
    }
    cout<<-1;
    return 0;
}