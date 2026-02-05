// BOJ 2268: 수들의 합 7

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

struct Fenwick_Tree{
    int N;
    vector<ll> vec;
    vector<ll> tree;

    Fenwick_Tree(int N): N(N){
        vec.resize(N+1, 0LL);
        tree.resize(N+1, 0LL);
    }

    void build(){
        for(int i(1); i<=N; ++i){
            tree[i]+=vec[i];
            int j(i+(i&(-i)));
            if(j<=N){
                tree[j]+=tree[i];
            }
        }
    }
    ll query(int index){
        ll sum(0LL);
        while(index>=1){
            sum+=tree[index];
            index-=(index&(-index));
        }
        return sum;
    }
    ll query_range(int qleft, int qright){
        return query(qright)-query(qleft-1);
    }
    void update_add(int index, ll delta_value){
        vec[index]+=delta_value;
        while(index<=N){
            tree[index]+=delta_value;
            index+=(index&(-index));
        }
    }
    void update_set(int index, ll value){
        update_add(index, value-vec[index]);
    }
};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int N, M; cin>>N>>M;
    Fenwick_Tree fwtr(N);
    while(M--){
        int type; cin>>type;
        if(type==0){
            int i, j; cin>>i>>j;
            if(i>j){
                swap(i, j);
            }
            cout<<fwtr.query_range(i, j)<<'\n';
        } elif(type==1){
            int i; ll k; cin>>i>>k;
            fwtr.update_set(i, k);
        }
    }

    return 0;
}