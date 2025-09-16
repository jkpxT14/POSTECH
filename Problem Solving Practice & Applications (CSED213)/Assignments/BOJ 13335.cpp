// BOJ 13335: 트럭

// #include <bits/stdc++.h>
#include <iostream>
#include <string>

#include <vector>
#include <stack>
#include <queue>
#include <deque> // DEQUE = Double-Ended QUEue
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

using namespace std;

using ll=long long;
using vi=vector<int>;
using vll=vector<long long>;
using vd=vector<double>;
using vb=vector<bool>;
using sti=stack<int>;
using stch=stack<char>;
using qi=queue<int>;
using dqi=deque<int>;
using dqll=deque<long long>;
using pii=pair<int, int>;
using vvi=vector<vector<int>>;
using vpii=vector<pair<int, int>>;
using vpipii=vector<pair<int, pair<int, int>>>;
using dqpii=deque<pair<int, int>>;
using vvpii=vector<vector<pair<int, int>>>;

#define INT_MAX 2147483647 // = 2^31-1
#define INT_MIN -2147483648
#define LLONG_MAX 9223372036854775807 // = 2^63-1
#define LLONG_MIN -9223372036854775808

// 0b(2), 0(8), 0x(16)
// 0-based index vs 1-based index

// const int INF=INT_MAX/4; // INF = INFinity

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    
    cout<<fixed;
    cout.precision(10);

    int n, w, L;
    cin>>n>>w>>L;
    dqpii ontb, offtb;
    for(int i=0; i<n; i++){
        int a;
        cin>>a;
        offtb.push_back({a, -1});
    }
    int cur_weight(0), ans(0);
    while(!ontb.empty() || !offtb.empty()){
        for(auto it=ontb.begin(); it!=ontb.end(); ++it) ++((*it).second);
        if(!ontb.empty() && ontb.front().second==w){
            cur_weight-=ontb.front().first;
            ontb.pop_front();
        }
        if(!offtb.empty() && offtb.front().first+cur_weight<=L){
            ontb.push_back({offtb.front().first, 0});
            cur_weight+=offtb.front().first;
            offtb.pop_front();
        }
        ++ans;
    }
    cout<<ans;

    return 0;
}