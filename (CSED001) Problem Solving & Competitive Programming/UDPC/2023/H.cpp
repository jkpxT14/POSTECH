// BOJ 27925: 인덕션

#include <bits/stdc++.h>

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
using vvb=vector<vector<bool>>;
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

using vvvb=vector<vector<vector<bool>>>;
using vvvvb=vector<vector<vector<vector<bool>>>>;
using vvvvi=vector<vector<vector<vector<int>>>>;

#define elif else if

const auto strnpos=string::npos;

// const int INF(INT_MAX/4);
// const ll mod(1000000007);
// const int offset(500000);
// const vpii delta{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

template <typename TN>
void print_2D(const vector<vector<TN>> &v, int type, int M, int N){
    for(int i(type); i<M+type; ++i){
        for(int j(type); j<N+type; ++j){
            cout<<v[i][j]<<' ';
        }
        cout<<'\n';
    }
}

int mod(int x, int modulus){ // modulo
    return ((x%modulus)+modulus)%modulus;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int N;
    cin>>N;
    vi t(N+1);
    for(int i(1); i<=N; ++i){
        cin>>t[i];
    }

    vvvvi dp(N+1, vvvi(10, vvi(10, vi(10, 25000))));
    dp[0][0][0][0]=0;
    for(int i(1); i<=N; ++i){
        for(int j(0); j<=9; ++j){
            for(int k(0); k<=9; ++k){
                for(int l(0); l<=9; ++l){
                    dp[i][t[i]][k][l]=min(dp[i][t[i]][k][l], dp[i-1][j][k][l]+min(mod(t[i]-j, 10), mod(j-t[i], 10)));
                    dp[i][j][t[i]][l]=min(dp[i][j][t[i]][l], dp[i-1][j][k][l]+min(mod(t[i]-k, 10), mod(k-t[i], 10)));
                    dp[i][j][k][t[i]]=min(dp[i][j][k][t[i]], dp[i-1][j][k][l]+min(mod(t[i]-l, 10), mod(l-t[i], 10)));
                }
            }
        }
    }
    int ans(25000);
    for(int j(0); j<=9; ++j){
        for(int k(0); k<=9; ++k){
            for(int l(0); l<=9; ++l){
                ans=min(ans, dp[N][j][k][l]);
            }
        }
    }
    cout<<ans;

    return 0;
}