// BOJ 14698: 전생했더니 슬라임 연구자였던 건에 대하여 (Hard)

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
using vvi=vector<vector<int>>;
using vpii=vector<pair<int, int>>;
using vpipii=vector<pair<int, pair<int, int>>>;
using dqpii=deque<pair<int, int>>;
using vvpii=vector<vector<pair<int, int>>>;

#define INT_MAX 2147483647 // = 2^31-1
#define INT_MIN -2147483648
#define LLONG_MAX 9223372036854775807 // = 2^63-1
#define LLONG_MIN -9223372036854775808

// const int INF=INT_MAX/4;
const ll mod=1000000007; // 10^9+7

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    
    cout<<fixed;
    cout.precision(10);

    int T;
    cin>>T;
    while(T--){
        int N;
        cin>>N;
        pqllg C;
        for(int i=0; i<N; ++i){
            ll D;
            cin>>D;
            C.push(D);
        }
        ll ans(1);
        while(C.size()>1){
            ll D(C.top()); C.pop();
            ll E(C.top()); C.pop();
            ans*=((D%mod)*(E%mod))%mod; ans%=mod;
            C.push(D*E);
        }
        cout<<ans<<"\n";
    }

    return 0;
}