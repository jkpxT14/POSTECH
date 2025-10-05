// Reference: BOJ 9663

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

// Promising
// Pruning

int N, cnt=0;
vi board;
vb column_check, positive_diagonal_check, negative_diagonal_check;

bool promising(int cur){
    // for(int i=1; i<cur; i++){
    //     if(board[cur]==board[i] || cur-i==abs(board[cur]-board[i])) return false;
    // }
    if(column_check[board[cur]] || positive_diagonal_check[board[cur]-cur+N] || negative_diagonal_check[board[cur]+cur-1]) return false;
    else return true;
}

void N_Queen(int cur){
    if(cur==N+1){
        cnt++;
        return;
    }
    for(int i=1; i<=N; i++){
        board[cur]=i;
        if(promising(cur)){
            column_check[board[cur]]=true;
            positive_diagonal_check[board[cur]-cur+N]=true;
            negative_diagonal_check[board[cur]+cur-1]=true;
            N_Queen(cur+1);
            column_check[board[cur]]=false;
            positive_diagonal_check[board[cur]-cur+N]=false;
            negative_diagonal_check[board[cur]+cur-1]=false;
        }
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    
    cout<<fixed;
    cout.precision(10);

    cin>>N;
    board.resize(N+1, 0);
    column_check.resize(N+1, false);
    positive_diagonal_check.resize(2*N, false);
    negative_diagonal_check.resize(2*N, false);

    N_Queen(1);
    cout<<cnt;

    return 0;
}