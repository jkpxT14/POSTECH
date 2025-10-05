// 막대기

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
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
// stack = sta = st
// queue = que = q
// deque = dq
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

    int N, cur_h(-1), cnt(0);
    cin>>N;
    vi h(N);
    for(int i=0; i<N; i++){
        cin>>h[i];
    }
    for(int i=N-1; i>=0; i--){
        if(h[i]>cur_h){
            cnt++;
            cur_h=h[i];
        }
    }
    cout<<cnt;

    return 0;
}