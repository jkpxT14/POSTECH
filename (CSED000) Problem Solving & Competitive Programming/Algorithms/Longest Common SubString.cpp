// Reference: BOJ 5582: 共通部分文字列(공통 부분 문자열)

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

    string s1, s2;
    cin>>s1>>s2;
    vvi dp(s1.size(), vi(s2.size(), 0));
    int ans(0);
    for(int i(0); i<s1.size(); ++i){
        if(s1[i]==s2[0]){
            dp[i][0]=1;
        }
    }
    for(int j(0); j<s2.size(); ++j){
        if(s1[0]==s2[j]){
            dp[0][j]=1;
        }
    }
    for(int i(1); i<s1.size(); ++i){
        for(int j(1); j<s2.size(); ++j){
            if(s1[i]==s2[j]){
                dp[i][j]=dp[i-1][j-1]+1;
                ans=max(ans, dp[i][j]);
            }
        }
    }
    cout<<ans;
    
    return 0;
}