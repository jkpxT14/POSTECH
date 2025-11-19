// BOJ 27920: 카드 뒤집기

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
#include <climits>
#include <limits>
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
using vstr=vector<string>;
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
using pll=pair<long long, long long>;
using ppiii=pair<pair<int, int>, int>;
using pipii=pair<int, pair<int, int>>;
using pci=pair<char, int>;
using vpii=vector<pair<int, int>>;
using vpci=vector<pair<char, int>>;
using vppiii=vector<pair<pair<int, int>, int>>;
using vpipii=vector<pair<int, pair<int, int>>>;
using dqpii=deque<pair<int, int>>;
using vvi=vector<vector<int>>;
using vvvi=vector<vector<vector<int>>>;
using vvll=vector<vector<long long>>;
using vvvll=vector<vector<vector<long long>>>;
using vvpii=vector<vector<pair<int, int>>>;

using mii=map<int, int>;

#define elif else if
const auto strnpos=string::npos;

// const int INF(INT_MAX);
// const ll mod(1000000007); // 10^9+7
// const int offset(500000);

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed;
    cout.precision(10);

    cout<<"YES"<<'\n';
    int N;
    cin>>N;
    dqi b;
    b.push_front(N);
    for(int i(1); i<=N-1; ++i){
        (i%2==0)?(b.push_front(i)):(b.push_back(i));
    }
    b.push_front(0);
    vi d(N+1);
    for(int i(1); i<=N; ++i){
        d[b[i]]=i;
    }
    vi c(N+1);
    for(int i(1); i<=N-1; ++i){
        c[i]=d[N-i];
    }
    c[N]=d[N];
    for(int i(1); i<=N; ++i){
        cout<<b[i]<<' ';
    }
    cout<<'\n';
    for(int i(1); i<=N; ++i){
        cout<<c[i]<<' ';
    }
    
    return 0;
}