// BOJ 25826: 2차원 배열 다중 업데이트 단일 합

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
using umii=unordered_map<int, int>;

using vvvb=vector<vector<vector<bool>>>;
using vvvvb=vector<vector<vector<vector<bool>>>>;
using vvvvi=vector<vector<vector<vector<int>>>>;
using vvstr=vector<vector<string>>;
using vpvii=vector<pair<vector<int>, int>>;
using tiiii=tuple<int, int, int, int>;
using dqtiiii=deque<tuple<int, int, int, int>>;

#define elif else if

const auto strnpos=string::npos;

template <typename TN1, typename TN2>
pair<TN1, TN2> operator+(const pair<TN1, TN2> &x, const pair<TN1, TN2> &y){
    return {x.first+y.first, x.second+y.second};
}

template <typename TN1, typename TN2>
pair<TN1, TN2> & operator+=(pair<TN1, TN2> &x, const pair<TN1, TN2> &y){
    x.first+=y.first;
    x.second+=y.second;
    return x;
}

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

// const int INF(INT_MAX/4);
// const ll mdls(1000000007LL);
// const int offset(500000);
// const vpii delta{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
// const vpii UDLR{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int n, m;
    cin>>n>>m;
    vvll A(n, vll(n));
    for(int i(0); i<n; ++i){
        for(int j(0); j<n; ++j){
            cin>>A[i][j];
        }
    }
    vvll IMOS(n+1, vll(n+1, 0));
    for(int i(0); i<m-1; ++i){
        int temp, i1, j1, i2, j2;
        ll k;
        cin>>temp>>i1>>j1>>i2>>j2>>k;
        IMOS[i1][j1]+=k;
        IMOS[i2+1][j2+1]+=k;
        IMOS[i1][j2+1]-=k;
        IMOS[i2+1][j1]-=k;
    }
    for(int i(1); i<n+1; ++i){
        for(int j(0); j<n+1; ++j){
            IMOS[i][j]+=IMOS[i-1][j];
        }
    }
    for(int i(0); i<n+1; ++i){
        for(int j(1); j<n+1; ++j){
            IMOS[i][j]+=IMOS[i][j-1];
        }
    }
    for(int i(0); i<n; ++i){
        for(int j(0); j<n; ++j){
            A[i][j]+=IMOS[i][j];
        }
    }
    int temp, i1, j1, i2, j2;
    cin>>temp>>i1>>j1>>i2>>j2;
    ll ans(0);
    for(int i(i1); i<=i2; ++i){
        for(int j(j1); j<=j2; ++j){
            ans+=A[i][j];
        }
    }
    cout<<ans;

    return 0;
}