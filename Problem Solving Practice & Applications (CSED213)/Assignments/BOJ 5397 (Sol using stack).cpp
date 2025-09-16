// BOJ 5397: 키로거

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
// vector = vec = v
// stack = sta = st // string = str( = st)
// queue = que = q
// deque = dq
// sol = solution
// ans = answer
// cur = current
// coordinate = coord
// adjacency matrix = adj_mat

// const int INF=INT_MAX/4; // INF = INFinity

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    
    cout<<fixed;
    cout.precision(10);

    int tc;
    cin>>tc;
    while(tc--){
        string s;
        cin>>s;
        int L=s.length();

        stch sta_left, sta_right;
        for(char c: s){
            if(c=='-'){
                if(!sta_left.empty()){
                    sta_left.pop();
                }
            }
            else if(c=='<'){
                if(!sta_left.empty()){
                    sta_right.push(sta_left.top());
                    sta_left.pop();
                }
            }
            else if(c=='>'){
                if(!sta_right.empty()){
                    sta_left.push(sta_right.top());
                    sta_right.pop();
                }
            }
            else{
                sta_left.push(c);
            }
        }
        while(!sta_right.empty()){
            sta_left.push(sta_right.top());
            sta_right.pop();
        }
        while(!sta_left.empty()){
            sta_right.push(sta_left.top());
            sta_left.pop();
        }
        while(!sta_right.empty()){
            cout<<sta_right.top();
            sta_right.pop();
        }
        cout<<"\n";
    }

    return 0;
}