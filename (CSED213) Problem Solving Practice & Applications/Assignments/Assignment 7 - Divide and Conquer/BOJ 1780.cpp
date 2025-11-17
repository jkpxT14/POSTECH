// BOJ 1780: 종이의 개수

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

int N;
vvi paper;
mii cnt;

void f(int x, int y, int size){
    for(int i(x); i<x+size; ++i){
        for(int j(y); j<y+size; ++j){
            if(paper[i][j]!=paper[x][y]){
                for(int k(0); k<3; ++k){
                    for(int l(0); l<3; ++l){
                        f(x+(size/3*k), y+(size/3*l), size/3);
                    }
                }
                return;
            }
        }
    }
    ++cnt[paper[x][y]];
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cout<<fixed;
    cout.precision(10);

    cin>>N;
    paper.resize(N, vi(N));
    for(int i(0); i<N; ++i){
        for(int j(0); j<N; ++j){
            cin>>paper[i][j];
        }
    }
    cnt[-1]=0; cnt[0]=0; cnt[1]=0;
    f(0, 0, N);
    cout<<cnt[-1]<<"\n"<<cnt[0]<<"\n"<<cnt[1];
    
    return 0;
}