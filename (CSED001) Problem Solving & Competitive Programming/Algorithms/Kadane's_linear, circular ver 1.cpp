//

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
using pci=pair<char, int>;
using vvi=vector<vector<int>>;
using vpii=vector<pair<int, int>>;
using vpci=vector<pair<char, int>>;
using vpipii=vector<pair<int, pair<int, int>>>;
using dqpii=deque<pair<int, int>>;
using vvpii=vector<vector<pair<int, int>>>;

#define INT_MAX 2147483647 // = 2^31-1 > 2*10^9
#define INT_MIN -2147483648
#define LLONG_MAX 9223372036854775807 // = 2^63-1 > 9*10^18
#define LLONG_MIN -9223372036854775808

// const int INF(INT_MAX/4);
// const ll mod(1000000007); // 10^9+7
// const int offset(500000);

int N;
vi v;
int KMl, Kml, KMc;

int Kadane_Max_linear(){
    vi cur(N);
    cur[0]=v[0];
    int ret(v[0]);
    for(int i=1; i<N; ++i){
        ret=max(ret, cur[i]=max(v[i], cur[i-1]+v[i]));
    }
    return ret;
}

int Kadane_min_linear(){
    vi cur(N);
    cur[0]=v[0];
    int ret(v[0]);
    for(int i=1; i<N; ++i){
        ret=min(ret, cur[i]=min(v[i], cur[i-1]+v[i]));
    }
    return ret;
}

int Kadane_Max_circular(){
    return KMl<0? KMl : max(KMl, accumulate(v.begin(), v.end(), 0)-Kml);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    
    cout<<fixed;
    cout.precision(10);

    cin>>N;
    v.resize(N);
    for(int i=0; i<N; ++i){
        cin>>v[i];
    }
    KMl=Kadane_Max_linear();
    Kml=Kadane_min_linear();
    KMc=Kadane_Max_circular();
    cout<<KMl<<" "<<Kml<<" "<<KMc;

    return 0;
}