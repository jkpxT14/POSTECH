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

using namespace std;

using ll=long long;
using vi=vector<int>;
using vll=vector<long long>;
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
// adjacency matrix = adj_mat

const int INF=INT_MAX/4; // INF = INFinity

int N, M, V;
vvi adj_mat;
vb visited;

void init()
{
    cin>>N>>M>>V;
    adj_mat.resize(N+1, vi(N+1, 0));
    visited.resize(N+1, false);

    for(int i=0; i<M; i++){
        int A, B;
        cin>>A>>B;
        adj_mat[A][B]=1;
        adj_mat[B][A]=1;
    }
}

void DFS()
{
    sti sta;
    bool deeper=false;

    sta.push(V);
    visited[V]=true;
    cout<<V<<" ";
    while(!sta.empty()){
        for(int i=1; i<=N; i++){
            if(adj_mat[sta.top()][i]==1 && !visited[i]){
                sta.push(i);
                visited[i]=true;
                cout<<i<<" ";
                deeper=true;
                break;
            }
        }
        if(deeper) deeper=false;
        else sta.pop();
    }
}

void BFS()
{
    qi que;
    int cur_node;

    que.push(V);
    visited[V]=true;
    cout<<V<<" ";
    while(!que.empty()){
        cur_node=que.front();
        que.pop();
        for(int i=1; i<=N; i++){
            if(adj_mat[cur_node][i]==1 && !visited[i]){
                que.push(i);
                visited[i]=true;
                cout<<i<<" ";
            }
        }
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    // cout.tie(nullptr);

    init();
    DFS();
    cout<<"\n";
    visited.assign(N+1, false);
    BFS();

    return 0;
}