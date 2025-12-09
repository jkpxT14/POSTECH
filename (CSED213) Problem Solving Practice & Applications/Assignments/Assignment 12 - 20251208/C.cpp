// BOJ 20303: 할로윈의 양아치

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
void print_1D(const vector<TN> &v, int type, int N){
    for(int i(type); i<N+type; ++i){
        cout<<v[i]<<' ';
    }
    cout<<'\n';
}

template <typename TN1, typename TN2>
void print_1D(const vector<pair<TN1, TN2>> &v, int type, int N){
    for(int i(type); i<N+type; ++i){
        cout<<'('<<v[i].first<<", "<<v[i].second<<") ";
    }
    cout<<'\n';
}

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

int N, M, K;
vi c;
vvi adj;
vb vst;
int cnt, sum;
vpii d;

void dfs(int x){
    vst[x]=true;
    ++cnt;
    sum+=c[x];
    for(int nx: adj[x]){
        if(!vst[nx]){
            dfs(nx);
        }
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    cin>>N>>M>>K;
    --K;
    c.resize(N+1);
    for(int i(1); i<=N; ++i){
        cin>>c[i];
    }
    adj.resize(N+1);
    for(int i(0); i<M; ++i){
        int a, b;
        cin>>a>>b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    vst.assign(N+1, false);
    for(int i(1); i<=N; ++i){
        if(!vst[i]){
            cnt=0;
            sum=0;
            dfs(i);
            d.push_back({cnt, sum});
        }
    }
    int D((int)d.size());
    d.insert(d.begin(), {0, 0});
    vvi dp(D+1, vi(K+1));
    for(int i(0); i<=D; ++i){
        dp[i][0]=0;
    }
    for(int j(0); j<=K; ++j){
        dp[0][j]=0;
    }
    for(int i(1); i<=D; ++i){
        for(int j(1); j<=K; ++j){
            if(d[i].first<=j){
                dp[i][j]=max(dp[i-1][j-d[i].first]+d[i].second, dp[i-1][j]);
            }
            else{
                dp[i][j]=dp[i-1][j];
            }
        }
    }
    cout<<dp[D][K];

    return 0;
}