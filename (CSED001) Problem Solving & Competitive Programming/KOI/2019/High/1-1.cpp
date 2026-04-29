// BOJ 17612, JUNGOL 3337: 쇼핑몰

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

struct customer{
    int id;
    int required;
    int counter;

    customer(int i, int r, int c): id(i), required(r), counter(c) {}
};

struct cmp_enter{
    bool operator()(const customer &A, const customer &B){
        if(A.required!=B.required){
            return A.required>B.required;
        }
        return A.counter>B.counter;
    }
};

struct cmp_exit{
    bool operator()(const customer &A, const customer &B){
        if(A.required!=B.required){
            return A.required>B.required;
        }
        return A.counter<B.counter;
    }
};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int N, k; cin>>N>>k;
    priority_queue<customer, vector<customer>, cmp_enter> enter;
    priority_queue<customer, vector<customer>, cmp_exit> exit;
    for(int i(1); i<=k; ++i){
        enter.push(customer(0, 0, i));
    }
    for(int i(1); i<=N; ++i){
        int id, w; cin>>id>>w;
        w+=enter.top().required;
        int counter(enter.top().counter);
        enter.pop();
        enter.push(customer(id, w, counter));
        exit.push(customer(id, w, counter));
    }
    ll ans(0);
    for(int i(1); i<=N; ++i){
        ans+=ll(i)*ll(exit.top().id); exit.pop();
    }
    cout<<ans;

    return 0;
}