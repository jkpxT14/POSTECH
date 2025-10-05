// Reference: BOJ 11003

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

#define INT_MAX 2147483647
#define INT_MIN -2147483648
#define LLONG_MAX 9223372036854775807
#define LLONG_MIN -9223372036854775808

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

    int N, L;
    cin>>N>>L;
    vi A(N+1);
    dqpii dq;
    for(int i=1; i<=N; i++){
        cin>>A[i];
    }
    
    for(int i=1; i<=N; i++){
        if(!dq.empty() && i>dq.front().second+L-1) dq.pop_front();
        while(!dq.empty() && dq.back().first>=A[i]) dq.pop_back();
        dq.push_back({A[i], i});
        cout<<dq.front().first<<" ";
    }

    return 0;
}