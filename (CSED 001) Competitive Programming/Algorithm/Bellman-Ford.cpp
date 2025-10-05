// Reference: BOJ 11657

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
using si=stack<int>;
using qi=queue<int>;
using dqi=deque<int>;
using dqll=deque<long long>;
using pii=pair<int, int>;
using vpii=vector<pair<int, int>>;
using vpipii=vector<pair<int, pair<int, int>>>;
using dqpii=deque<pair<int, int>>;
using vvpii=vector<vector<pair<int, int>>>;

#define INT_MAX 2147483647
#define INT_MIN -2147483648
#define LLONG_MAX 9223372036854775807
#define LLONG_MIN -9223372036854775808

// ans = answer
// cur = current

const ll INF=LLONG_MAX;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    // cout.tie(nullptr);

    int N, M;
    cin>>N>>M;
    
    vll dist(N+1, INF);
    vpipii edge(M);

    for(int i=0; i<M; i++){
        int A, B, C;
        cin>>A>>B>>C;
        edge[i]={C, {A, B}};
    }

    dist[1]=0;
    for(int i=1; i<=N; i++){
        for(auto &cur_edge: edge){
            if(dist[cur_edge.second.first]!=INF){
                if(dist[cur_edge.second.second]>dist[cur_edge.second.first]+(ll)cur_edge.first){
                    if(i==N){
                        cout<<"-1";
                        return 0;
                    }
                    dist[cur_edge.second.second]=dist[cur_edge.second.first]+(ll)cur_edge.first;
                }
            }
        }
    }

    for(int i=2; i<=N; i++){
        if(dist[i]==INF) dist[i]=-1;
        cout<<dist[i]<<"\n";
    }

    return 0;
}