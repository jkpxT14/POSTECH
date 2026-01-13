// BOJ 27923: 햄버거최대 몇개드실수있나요?

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

#define elif else if
const auto strnpos=string::npos;

// const int INF(INT_MAX);
// const ll mod(1000000007); // 10^9+7
// const int offset(500000);

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed;
    cout.precision(10);

    int N, K, L;
    cin>>N>>K>>L;
    vll m(N+1), t(K+1);
    for(int i(1); i<=N; ++i) cin>>m[i];
    for(int i(1); i<=K; ++i) cin>>t[i];
    vll C(N+1, 0);
    for(int i(1); i<=K; ++i) ++C[t[i]];
    // for(int i(1); i<=N-L+1; ++i){
    //     for(int j(1); j<=L-1; ++j){
    //         C[i+j]+=C[i];
    //     }
    // }
    vll D(N+1);
    ll cur(0);
    for(int i(1); i<=N; ++i){
        cur+=C[i];
        if(i>=L+1){
            cur-=C[i-L];
        }
        D[i]=cur;
    }
    sort(m.begin()+1, m.end(), greater<ll>());
    sort(D.begin()+1, D.end(), greater<ll>());
    ll ans(0);
    for(int i(1); i<=N; ++i){
        ans+=(ll)(m[i]/(ll)pow(2, D[i]));
    }
    cout<<ans;
    
    return 0;
}