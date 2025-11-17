// BOJ 1931: 회의실 배정

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
// using sti=stack<int>;
// using stch=stack<char>;
// using qi=queue<int>;
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

// const int INF=INT_MAX/4;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    
    cout<<fixed;
    cout.precision(10);

    int N;
    cin>>N;
    vpii I(N);
    for(int i=0; i<N; ++i){
        cin>>I[i].first>>I[i].second;
    }
    sort(I.begin(), I.end(), [](pii x, pii y){
        if(x.second!=y.second) return x.second<y.second;
        return x.first<y.first;
    });
    int ans(0), end(0);
    for(int i=0; i<N; ++i){
        if(I[i].first>=end){
            ++ans;
            end=I[i].second;
        }
    }
    cout<<ans;

    return 0;
}