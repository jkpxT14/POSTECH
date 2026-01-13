//

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
using ppiii=pair<pair<int, int>, int>;
using pipii=pair<int, pair<int, int>>;
using pci=pair<char, int>;
using vpii=vector<pair<int, int>>;
using vpci=vector<pair<char, int>>;
using vppiii=vector<pair<pair<int, int>, int>>;
using vpipii=vector<pair<int, pair<int, int>>>;
using dqpii=deque<pair<int, int>>;
using vvi=vector<vector<int>>;
using vvll=vector<vector<long long>>;
using vvpii=vector<vector<pair<int, int>>>;

using mii=map<int, int>;

#define elif else if

// const int INF(INT_MAX);
// const ll mod(1000000007); // 10^9+7
// const int offset(500000);

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed;
    cout.precision(10);

    int V, E;
    cin>>V>>E;
    vvi DAG(V+1);
    vi in_deg(V+1, 0), out_deg(V+1, 0);
    for(int i(0); i<E; ++i){
        int from, to;
        cin>>from>>to;
        DAG[from].push_back(to);
        ++in_deg[to];
        ++out_deg[from];
    }
    dqi q;
    for(int i(1); i<=V; ++i){
        if(in_deg[i]==0){
            q.push_back(i);
        }
    }
    for(int i(0); i<V; ++i){
        // assert(!q.empty());
        if(q.empty()){
            cout<<"cycle";
            return 0;
        }
        int cur(q.front());
        cout<<cur<<" ";
        q.pop_front();
        for(int j(0); j<DAG[cur].size(); ++j){
            int next(DAG[cur][j]);
            if(--in_deg[next]==0){
                q.push_back(next);
            }
        }
    }
    
    return 0;
}