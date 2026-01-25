// Reference: BOJ 11404

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
using vvi=vector<vector<int>>;
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

const int INF=INT_MAX/4;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    // cout.tie(nullptr);

    int n, m;
    cin>>n;
    vvi dist(n+1, vi(n+1, INF));
    for(int i=1; i<=n; i++){
        dist[i][i]=0;
    }
    cin>>m;
    for(int i=0; i<m; i++){
        int a, b, c;
        cin>>a>>b>>c;
        dist[a][b]=min(dist[a][b], c);
    }
    for(int i=1; i<=n; i++){
        for(int j=1; j<=n; j++){
            for(int k=1; k<=n; k++){
                dist[j][k]=min(dist[j][k], dist[j][i]+dist[i][k]);
            }
        }
    }
    for(int i=1; i<=n; i++){
        for(int j=1; j<=n; j++){
            cout<<(dist[i][j]==INF ? 0 : dist[i][j])<<" ";
        }
        cout<<"\n";
    }

    return 0;
}