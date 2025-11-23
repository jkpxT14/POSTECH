// Reference: BOJ 6574: Advanced Fruits(새로운 과일)

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
using pii=pair<int, int>;
using pll=pair<long long, long long>;
using pci=pair<char, int>;
using pipii=pair<int, pair<int, int>>;
using ppiii=pair<pair<int, int>, int>;
using vi=vector<int>;
using vll=vector<long long>;
using vd=vector<double>;
using vb=vector<bool>;
using vc=vector<char>;
using vstr=vector<string>;
using vpii=vector<pair<int, int>>;
using vpll=vector<pair<long long, long long>>;
using vpci=vector<pair<char, int>>;
using vpipii=vector<pair<int, pair<int, int>>>;
using vppiii=vector<pair<pair<int, int>, int>>;
using vvi=vector<vector<int>>;
using vvll=vector<vector<long long>>;
using vvc=vector<vector<char>>;
using vvpii=vector<vector<pair<int, int>>>;
using vvvi=vector<vector<vector<int>>>;
using vvvll=vector<vector<vector<long long>>>;
using dqi=deque<int>;
using dqll=deque<long long>;
using dqpii=deque<pair<int, int>>;
using pqi=priority_queue<int>;
using pqig=priority_queue<int, vector<int>, greater<int>>;
using pqll=priority_queue<long long>;
using pqllg=priority_queue<long long, vector<long long>, greater<long long>>;
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

    string s1, s2;
    while(cin>>s1>>s2){
        int l1((int)s1.size()), l2((int)s2.size());
        
        vvi dp(l1+1, vi(l2+1, 0));
        for(int i(1); i<=l1; ++i){
            for(int j(1); j<=l2; ++j){
                dp[i][j]=(s1[i-1]==s2[j-1])?(dp[i-1][j-1]+1):max(dp[i-1][j], dp[i][j-1]);
            }
        }

        string SCS("");
        int i(l1), j(l2);
        while(i>=1 && j>=1){
            if(s1[i-1]==s2[j-1]){
                SCS+=s1[i-1];
                --i;
                --j;
            }
            elif(dp[i-1][j]>=dp[i][j-1]){
                SCS+=s1[i-1];
                --i;
            }
            else{
                SCS+=s2[j-1];
                --j;
            }
        }
        while(i>=1){
            SCS+=s1[i-1];
            --i;
        }
        while(j>=1){
            SCS+=s2[j-1];
            --j;
        }
        reverse(SCS.begin(), SCS.end());
        cout<<SCS<<'\n';
    }

    return 0;
}