// BOJ 2096: 내려가기

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

    int N;
    cin>>N;
    vi dp_max(3);
    for(int i(0); i<3; ++i) cin>>dp_max[i];
    vi dp_min(dp_max);
    for(int i(1); i<N; ++i){
        vi line(3);
        for(int j(0); j<3; ++j) cin>>line[j];

        vi prev(3);

        prev=dp_max;
        dp_max[0]=max(prev[0], prev[1])+line[0];
        dp_max[1]=max(prev[0], max(prev[1], prev[2]))+line[1];
        dp_max[2]=max(prev[1], prev[2])+line[2];

        prev=dp_min;
        dp_min[0]=min(prev[0], prev[1])+line[0];
        dp_min[1]=min(prev[0], min(prev[1], prev[2]))+line[1];
        dp_min[2]=min(prev[1], prev[2])+line[2];
    }
    cout<<*max_element(dp_max.begin(), dp_max.end())<<" "<<*min_element(dp_min.begin(), dp_min.end());
    
    return 0;
}