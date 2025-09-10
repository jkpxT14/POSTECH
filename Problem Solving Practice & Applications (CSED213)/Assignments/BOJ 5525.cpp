// BOJ 5525: IOIOI

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
// cnt = count
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

    int N, M;
    string S;
    cin>>N;
    cin>>M;
    cin>>S;
    int ans=0;
    for(int i=0; i<M; ++i){
        int cnt=0;
        if(S[i]=='O') continue;
        while(S[i+1]=='O' && S[i+2]=='I'){
            ++cnt;
            if(cnt==N){
                ++ans;
                --cnt;
            }
            i+=2;
        }
    }
    cout<<ans;

    return 0;
}