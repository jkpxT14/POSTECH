// Reference: BOJ 2003

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
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
    // cout.tie(nullptr);
    
    cout<<fixed;
    cout.precision(10);

    int N, M;
    cin>>N>>M;
    vi A(N+1);
    for(int i=1; i<=N; i++){
        cin>>A[i];
    }
    int ans(0), sum(A[1]), l(1), r(1);
    while(r<=N){
        if(sum==M){
            ans++;
            sum-=A[l];
            l++;
            r++;
            if(r>N) break;
            else sum+=A[r];
        }
        else if(sum<M){
            r++;
            if(r>N) break;
            else sum+=A[r];
        }
        else{
            sum-=A[l];
            l++;
        }
    }
    cout<<ans;

    return 0;
}