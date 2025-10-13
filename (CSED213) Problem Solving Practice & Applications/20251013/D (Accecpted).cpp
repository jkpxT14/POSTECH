// BOJ 30090: 백신 개발

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
using vvi=vector<vector<int>>;
using vpii=vector<pair<int, int>>;
using vpci=vector<pair<char, int>>;
using vppiii=vector<pair<pair<int, int>, int>>;
using vpipii=vector<pair<int, pair<int, int>>>;
using dqpii=deque<pair<int, int>>;
using vvpii=vector<vector<pair<int, int>>>;

#define elif else if

// const int INF(INT_MAX/4);
// const ll mod(1000000007); // 10^9+7
// const int offset(500000);

int overlap(const string& x, const string& y){
    int l=min((int)x.size(), (int)y.size());
    for(int i=l; i>0; --i){
        if(x.compare((int)x.size()-i, i, y, 0, i)==0) return i;
    }
    return 0;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    
    cout<<fixed;
    cout.precision(10);

    int N;
    cin>>N;
    vstr virus(N);
    for(int i=0; i<N; ++i) cin>>virus[i];
    vi permutation(N);
    iota(permutation.begin(), permutation.end(), 0);
    int ans(INT_MAX);
    do{
        string cur=virus[permutation[0]];
        for(int i=1; i<N; ++i){
            cur+=virus[permutation[i]].substr(overlap(cur, virus[permutation[i]]));
        }
        ans=min(ans, (int)cur.size());
    } while(next_permutation(permutation.begin(), permutation.end()));

    cout<<ans;

    return 0;
}