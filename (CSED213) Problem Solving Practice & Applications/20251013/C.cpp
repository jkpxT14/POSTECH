// BOJ 8980: 택배

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
using ppiii=pair<pair<int, int>, int>;
using pipii=pair<int, pair<int, int>>;
using pci=pair<char, int>;
using vvi=vector<vector<int>>;
using vpii=vector<pair<int, int>>;
using vpci=vector<pair<char, int>>;
using vppiii=vector<pair<pair<int, int>, int>>;
using vpipii=vector<pair<int, pair<int, int>>>;
using dqpii=deque<pair<int, int>>;
using vvpii=vector<vector<pair<int, int>>>;

#define elif else if

#define INT_MAX 2147483647 // = 2^31-1 > 2*10^9
#define INT_MIN -2147483648
#define LLONG_MAX 9223372036854775807 // = 2^63-1 > 9*10^18
#define LLONG_MIN -9223372036854775808

// const int INF(INT_MAX/4);
// const ll mod(1000000007); // 10^9+7
// const int offset(500000);

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    
    cout<<fixed;
    cout.precision(10);

    int N, C;
    cin>>N>>C;
    vi box(N+1, 0);
    int M;
    cin>>M;
    vppiii order(M);
    for(int i=0; i<M; ++i){
        cin>>order[i].first.first>>order[i].first.second>>order[i].second;
    }
    sort(order.begin(), order.end(), [](const ppiii& x, const ppiii& y){
        if(x.first.second!=y.first.second) return x.first.second<y.first.second;
        return x.first.first<y.first.first;
    });
    int ans(0);
    for(int i=0; i<M; ++i){
        int cur(0);
        for(int j=order[i].first.first; j<order[i].first.second; ++j) cur=max(cur, box[j]);
        int next=(cur+order[i].second)<=C?order[i].second:C-cur;
        for(int j=order[i].first.first; j<order[i].first.second; ++j) box[j]+=next;
        ans+=next;
    }
    cout<<ans;

    return 0;
}