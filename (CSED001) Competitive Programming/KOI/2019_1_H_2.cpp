// BOJ 17613: 점프

// #include <bits/stdc++.h>

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <numeric>
#include <utility>
#include <tuple>
// #include <limits>
#include <cmath>
#include <bitset>
#include <functional>
#include <cassert>

using namespace std;

using ll=long long;
using vi=vector<int>;
using vll=vector<long long>;
using vd=vector<double>;
using vb=vector<bool>;
// using sti=stack<int>;
// using stch=stack<char>;
// using qi=queue<int>;
using dqi=deque<int>;
using dqll=deque<long long>;
using pqi=priority_queue<int>;
using pqig=priority_queue<int, vector<int>, greater<int>>;
using pqll=priority_queue<long long>;
using pqllg=priority_queue<long long, vector<long long>, greater<long long>>;
using pii=pair<int, int>;
using vvi=vector<vector<int>>;
using vpii=vector<pair<int, int>>;
using vpipii=vector<pair<int, pair<int, int>>>;
using dqpii=deque<pair<int, int>>;
using vvpii=vector<vector<pair<int, int>>>;

#define INT_MAX 2147483647 // = 2^31-1
#define INT_MIN -2147483648
#define LLONG_MAX 9223372036854775807 // = 2^63-1
#define LLONG_MIN -9223372036854775808

// const int INF(INT_MAX/4);
// const ll mod(1000000007); // 10^9+7
// const int offset(500000);

map<pii, int> dp_1;
map<pii, int> dp_2;

int w_1(int p, int q){
    if(dp_1.find({p, q})!=dp_1.end()) return dp_1[{p, q}];
    int ret(-1);
    int k(0);
    pii block({0, 0}); // [2^k-1, 2^(k+1)-2]
    while(block.first>q || q>block.second){
        ++k;
        block.first=((block.first+1)<<1)-1;
        block.second=((block.second+2)<<1)-2;
    }
    if(block.first<=p && p<=block.second) ret=k+w_1(p-block.first, q-block.first);
    else ret=max(w_1(p, block.first-1), k+w_1(0, q-block.first));
    return dp_1[{p, q}]=ret;
}

int w_2(int p, int q){
    if(dp_2.find({p, q})!=dp_2.end()) return dp_2[{p, q}];
    int ret(-1);
    for(int i=1; i<=30; ++i){
        int r=(1<<i)-1;
        int s=(1<<(i+1))-2;
        r=max(p, r); s=min(q, s);
        if(r<=s){
            int u=r-(1<<i)+1;
            int v=s-(1<<i)+1;
            ret=max(ret, w_2(u, v)+i);
        }
    }
    return dp_2[{p, q}]=ret;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    
    cout<<fixed;
    cout.precision(10);

    dp_1.insert({{0, 0}, 0}); // dp_1[{0, 0}]=0;
    dp_2.insert({{0, 0}, 0}); // dp_2[{0, 0}]=0;
    int T;
    cin>>T;
    while(T--){
        int x, y;
        cin>>x>>y;
        cout<<w_1(x, y)<<" "<<w_2(x, y)<<"\n";
    }

    return 0;
}