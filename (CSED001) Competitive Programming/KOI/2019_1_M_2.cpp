// BOJ 17611: 직각다각형

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
const int offset(500000);

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    
    cout<<fixed;
    cout.precision(10);

    int n;
    cin>>n;
    vpii coord(n+1);
    for(int i=0; i<n; ++i){
        int x, y;
        cin>>x>>y;
        coord[i]={x+offset, y+offset};
    }
    coord[n]=coord[0];
    vi h(2*offset+1, 0), v(2*offset+1, 0);
    for(int i=0; i<n; ++i){
        if(coord[i].first==coord[i+1].first){
            ++h[min(coord[i].second, coord[i+1].second)];
            --h[max(coord[i].second, coord[i+1].second)];
        }
        else if(coord[i].second==coord[i+1].second){
            ++v[min(coord[i].first, coord[i+1].first)];
            --v[max(coord[i].first, coord[i+1].first)];
        }
    }
    for(int i=1; i<2*offset+1; ++i){
        h[i]+=h[i-1];
        v[i]+=v[i-1];
    }
    int ans(-1);
    for(int i=0; i<2*offset+1; ++i){
        ans=max({ans, h[i], v[i]});
    }
    cout<<ans;

    return 0;
}