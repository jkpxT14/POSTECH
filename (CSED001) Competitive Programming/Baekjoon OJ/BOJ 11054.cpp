// BOJ 11054: 가장 긴 바이토닉 부분 수열

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
    
    int N;
    cin>>N;
    vi A(N);
    for(int i=0; i<N; ++i) cin>>A[i];
    vi dp_left(N, 1), dp_right(N, 1), dp(N);
    for(int i=0; i<N; ++i){
        for(int j=0; j<i; ++j){
            if(A[i]>A[j]) dp_left[i]=max(dp_left[i], dp_left[j]+1);
        }
    }
    for(int i=N-1; i>-1; --i){
        for(int j=N-1; j>i; --j){
            if(A[i]>A[j]) dp_right[i]=max(dp_right[i], dp_right[j]+1);
        }
    }
    int ans(0);
    for(int i=0; i<N; ++i){
        dp[i]=dp_left[i]+dp_right[i];
        ans=max(ans, dp[i]);
    }
    --ans;
    cout<<ans;

    return 0;
}