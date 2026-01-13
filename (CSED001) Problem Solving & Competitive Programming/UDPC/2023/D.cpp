// BOJ 27921: 동전 퍼즐

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

    vvc before(30, vc(30, '.')), after(30, vc(30, '.'));
    int H, W;
    cin>>H>>W;
    for(int i(0); i<H; ++i){
        for(int j(0); j<W; ++j){
            cin>>before[i][j];
        }
    }
    cin>>H>>W;
    for(int i(10); i<10+H; ++i){
        for(int j(10); j<10+W; ++j){
            cin>>after[i][j];
        }
    }

    // for(int i(0); i<30; ++i){
    //     for(int j(0); j<30; ++j){
    //         cout<<after[i][j]<<' ';
    //     }
    //     cout<<'\n';
    // }
    // cout<<'\n';

    pii p{29, 0}, q{29, 0};
    for(int i(0); i<30; ++i){
        for(int j(0); j<30; ++j){
            if(before[i][j]=='O'){
                q.first=min(q.first, j);
                q.second=max(q.second, j);
            }
            if(before[j][i]=='O'){
                p.first=min(p.first, j);
                p.second=max(p.second, j);
            }
        }
    }
    pii shift{29-p.second+p.first, 29-q.second+q.first};

    vvc original(30, vc(30, '.'));
    for(int i(0); i<30; ++i){
        for(int j(0); j<30; ++j){
            if(before[i][j]=='O'){
                original[i-p.first][j-q.first]='O';
            }
        }
    }

    // cout<<p.first<<' '<<p.second<<' '<<q.first<<' '<<q.second<<'\n';
    // for(int i(0); i<30; ++i){
    //     for(int j(0); j<30; ++j){
    //         cout<<original[i][j]<<' ';
    //     }
    //     cout<<'\n';
    // }
    // cout<<'\n';

    int ans(100);
    for(int i(0); i<=shift.first; ++i){
        for(int j(0); j<=shift.second; ++j){
            before.assign(30, vc(30, '.'));
            for(int k(0); k<30; ++k){
                for(int l(0); l<30; ++l){
                    if(original[k][l]=='O'){
                        before[k+i][l+j]='O';
                    }
                }
            }

            // for(int k(0); k<30; ++k){
            //     for(int l(0); l<30; ++l){
            //         cout<<before[k][l]<<' ';
            //     }
            //     cout<<'\n';
            // }

            int cnt(0);
            for(int k(0); k<30; ++k){
                for(int l(0); l<30; ++l){
                    if(before[k][l]!=after[k][l]){
                        ++cnt;
                    }
                }
            }
            assert(cnt%2==0);
            // cout<<cnt/2<<'\n';
            ans=min(ans, cnt/2);
        }
    }
    cout<<ans;
    
    return 0;
}