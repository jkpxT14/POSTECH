// BOJ 26042: 식당 입구 대기 줄

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

#define INT_MAX 2147483647 // = 2^31-1 > 2*10^9
#define INT_MIN -2147483648
#define LLONG_MAX 9223372036854775807 // = 2^63-1 > 9*10^18
#define LLONG_MIN -9223372036854775808

// const int INF(INT_MAX/4);
// const ll mod(1000000007); // 10^9+7
// const int offset(500000);

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    
    cout<<fixed;
    cout.precision(10);

    int n;
    cin>>n;
    dqi stu;
    pii ans;
    for(int i=0; i<n; ++i){
        int m;
        cin>>m;
        if(m==1){
            int a;
            cin>>a;
            stu.push_back(a);
        }
        else if(m==2) stu.pop_front();
        if(ans.first<stu.size()){
            ans.first=stu.size();
            ans.second=stu.back();
        }
        elif(ans.first==stu.size()) ans.second=min(ans.second, stu.back());
    }
    cout<<ans.first<<" "<<ans.second;

    return 0;
}