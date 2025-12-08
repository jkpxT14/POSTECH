// BOJ 30090: 백신 개발

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

int overlap(const string& x, const string& y){
    int l=min((int)x.size(), (int)y.size());
    for(int i=l; i>0; --i){
        if(x.compare((int)x.size()-i, i, y, 0, i)==0) return i;
    }
    return 0;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    
    cout<<fixed;
    cout.precision(10);

    int N;
    cin>>N;
    vstr virus(N);
    for(int i=0; i<N; ++i) cin>>virus[i];

    sort(virus.begin(), virus.end());
    virus.erase(unique(virus.begin(), virus.end()), virus.end());
    vstr kept;
    for(int i=0; i<(int)virus.size(); ++i){
        bool sub(false);
        for(int j=0; j<(int)virus.size(); ++j){
            if(i!=j){
                if(virus[j].find(virus[i])!=string::npos){
                    sub=true;
                    break;
                }
            }
        }
        if(!sub) kept.push_back(virus[i]);
    }
    virus.swap(kept);
    N=(int)virus.size();

    vvi over(N, vi(N));
    for(int i=0; i<N; ++i){
        for(int j=0; j<N; ++j){
            if(i!=j) over[i][j]=overlap(virus[i], virus[j]);
        }
    }
    vi permutation(N);
    iota(permutation.begin(), permutation.end(), 0);
    int ans(INT_MAX);
    do{
        int len(0);
        for(int i=0; i<N; ++i) len+=(int)virus[permutation[i]].size();
        for(int i=0; i<N-1; ++i) len-=over[permutation[i]][permutation[i+1]];
        ans=min(ans, len);
    } while(next_permutation(permutation.begin(), permutation.end()));

    cout<<ans;

    return 0;
}