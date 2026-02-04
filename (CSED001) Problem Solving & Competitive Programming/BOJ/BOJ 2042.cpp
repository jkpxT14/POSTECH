// BOJ 2042: 구간 합 구하기

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

struct Segment_Tree{
    int n;
    vector<ll> tree;

    Segment_Tree(int n): n(n){
        tree.resize(1<<(int(ceil(log2(n)))+1), 0LL);
    }

    void build(const vector<ll> &vec, int node, int left, int right){
        if(left==right){
            tree[node]=vec[left];
            return;
        }
        int mid((left+right)>>1);
        build(vec, 2*node, left, mid);
        build(vec, 2*node+1, mid+1, right);
        tree[node]=tree[2*node]+tree[2*node+1];
        return;
    }
    ll query(int node, int left, int right, int qleft, int qright){
        if(qright<left || qleft>right){
            return 0LL;
        }
        if(qleft<=left && right<=qright){
            return tree[node];
        }
        int mid((left+right)>>1);
        return query(2*node, left, mid, qleft, qright)+query(2*node+1, mid+1, right, qleft, qright);
    }
    void update(int node, int left, int right, int index, ll value){
        if(left==right){
            tree[node]=value;
            return;
        }
        int mid((left+right)>>1);
        if(index<=mid){
            update(2*node, left, mid, index, value);
        } else{
            update(2*node+1, mid+1, right, index, value);
        }
        tree[node]=tree[2*node]+tree[2*node+1];
        return;
    }
    void build(const vector<ll> &vec){
        build(vec, 1, 1, n);
    }
    ll query(int qleft, int qright){
        return query(1, 1, n, qleft, qright);
    }
    void update(int index, ll value){
        update(1, 1, n, index, value);
    }
};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int N, M, K; cin>>N>>M>>K;
    vector<ll> v(N+1);
    for(int i(1); i<=N; ++i){
        cin>>v[i];
    }
    Segment_Tree sgtr(N);
    sgtr.build(v);
    for(int i(1); i<=M+K; ++i){
        ll a, b, c; cin>>a>>b>>c;
        if(int(a)==1){
            sgtr.update(int(b), c);
        } elif(int(a)==2){
            cout<<sgtr.query(int(b), int(c))<<'\n';
        }
    }

    return 0;
}