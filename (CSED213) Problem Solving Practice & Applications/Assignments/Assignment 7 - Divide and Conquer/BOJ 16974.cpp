// BOJ 16974: 레벨 햄버거

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

#define elif else if

// const int INF(INT_MAX);
// const ll mod(1000000007); // 10^9+7
// const int offset(500000);

int N;
ll X;
vll burger, bun, patty;

ll eat_burger(int lv, ll num){
    if(lv==0){
        assert(num==1);
        return 1;
    }
    elif(num==1) return 0;
    elif(1<num && num<=1+burger[lv-1]) return eat_burger(lv-1, num-1);
    elif(num==1+burger[lv-1]+1) return patty[lv-1]+1;
    elif(1+burger[lv-1]+1<num && num<=1+burger[lv-1]+1+burger[lv-1]) return patty[lv-1]+1+eat_burger(lv-1, num-(1+burger[lv-1]+1));
    elif(num==1+burger[lv-1]+1+burger[lv-1]+1) return patty[lv];
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cout<<fixed;
    cout.precision(10);

    cin>>N>>X;
    burger.resize(N+1); bun.resize(N+1); patty.resize(N+1);
    burger[0]=1; bun[0]=0; patty[0]=1;
    for(int i=1; i<=N; ++i){
        burger[i]=1+burger[i-1]+1+burger[i-1]+1;
        bun[i]=1+bun[i-1]+bun[i-1]+1;
        patty[i]=patty[i-1]+1+patty[i-1];
    }
    cout<<eat_burger(N, X);
    
    return 0;
}