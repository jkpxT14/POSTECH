// BOJ 17613, JUNGOL 3338: 점프

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

map<pair<int, int>, int> dp_1;
map<pair<int, int>, int> dp_2;

int w_1(int p, int q){
    if(dp_1.find({p, q})!=dp_1.end()){
        return dp_1[{p, q}];
    }
    int ret(-1);
    int k(0);
    pair<int, int> block({0, 0});
    while(block.first>q || q>block.second){
        ++k;
        block.first=((block.first+1)<<1)-1;
        block.second=((block.second+2)<<1)-2;
    }
    if(block.first<=p && p<=block.second){
        ret=k+w_1(p-block.first, q-block.first);
    } else{
        ret=max(w_1(p, block.first-1), k+w_1(0, q-block.first));
    }
    return dp_1[{p, q}]=ret;
}

int w_2(int p, int q){
    if(dp_2.find({p, q})!=dp_2.end()){
        return dp_2[{p, q}];
    }
    int ret(-1);
    for(int i(1); i<=30; ++i){
        int r((1<<i)-1), s((1<<(i+1))-2);
        r=max(p, r); s=min(q, s);
        if(r<=s){
            int u(r-(1<<i)+1), v(s-(1<<i)+1);
            ret=max(ret, i+w_2(u, v));
        }
    }
    return dp_2[{p, q}]=ret;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    dp_1.insert({{0, 0}, 0});
    dp_2.insert({{0, 0}, 0});
    int T; cin>>T;
    while(T--){
        int x, y; cin>>x>>y;
        cout<<w_1(x, y)<<' '<<w_2(x, y)<<'\n';
    }

    return 0;
}