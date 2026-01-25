// BOJ 17616: 등수 찾기

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
using pci=pair<char, int>;
using vvi=vector<vector<int>>;
using vpii=vector<pair<int, int>>;
using vpci=vector<pair<char, int>>;
using vpipii=vector<pair<int, pair<int, int>>>;
using dqpii=deque<pair<int, int>>;
using vvpii=vector<vector<pair<int, int>>>;

#define INT_MAX 2147483647 // = 2^31-1 > 2*10^9
#define INT_MIN -2147483648
#define LLONG_MAX 9223372036854775807 // = 2^63-1 > 9*10^18
#define LLONG_MIN -9223372036854775808

// const int INF(INT_MAX/4);
// const ll mod(1000000007); // 10^9+7
// const int offset(500000);

vvi adj, radj;
vb visited;

int dfs(int cur){
    int ret(1);
    visited[cur]=true;
    for(int next: adj[cur]){
        if(!visited[next]) ret+=dfs(next);
    }
    return ret;
}

int rdfs(int cur){
    int ret(1);
    visited[cur]=true;
    for(int next: radj[cur]){
        if(!visited[next]) ret+=rdfs(next);
    }
    return ret;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    
    cout<<fixed;
    cout.precision(10);

    int N, M, X;
    cin>>N>>M>>X;
    adj.resize(N+1);
    radj.resize(N+1);
    for(int i=0; i<M; ++i){
        int A, B;
        cin>>A>>B;
        adj[B].push_back(A);
        radj[A].push_back(B);
    }
    visited.assign(N+1, false);
    int U(dfs(X));
    visited.assign(N+1, false);
    int V(N-rdfs(X)+1);
    cout<<U<<" "<<V;
    return 0;
}