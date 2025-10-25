// BOJ 9251: LCS

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

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    
    cout<<fixed;
    cout.precision(10);

    string s_1, s_2;
    cin>>s_1>>s_2;
    int M((int)s_1.size()), N((int)s_2.size());
    s_1.insert(0, " ");
    s_2.insert(0, " ");
    vvi dp(M+1, vi(N+1));
    for(int i=0; i<=M; ++i) dp[i][0]=0;
    for(int j=0; j<=N; ++j) dp[0][j]=0;
    for(int i=1; i<=M; ++i){
        for(int j=1; j<=N; ++j){
            dp[i][j]=(s_1[i]==s_2[j])?dp[i-1][j-1]+1:max(dp[i-1][j], dp[i][j-1]);
        }
    }
    cout<<dp[M][N];

    return 0;
}