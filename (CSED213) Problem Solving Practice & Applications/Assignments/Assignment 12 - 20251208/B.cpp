// BOJ 24460: 특별상이라도 받고 싶어

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

int N;
vvi lottery;

int g(int a, int b, int c, int d){
    vi v{a, b, c, d};
    sort(v.begin(), v.end());
    return v[1];
}

int f(int x, int y, int n){
    if(n==1){
        return lottery[x][y];
    }
    if(n==2){
        return g(lottery[x][y], lottery[x][y+1], lottery[x+1][y], lottery[x+1][y+1]);
    }
    return g(f(x, y, n/2), f(x, y+n/2, n/2), f(x+n/2, y, n/2), f(x+n/2, y+n/2, n/2));
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    cin>>N;
    lottery.resize(N, vi(N));
    for(int i(0); i<N; ++i){
        for(int j(0); j<N; ++j){
            cin>>lottery[i][j];
        }
    }
    cout<<f(0, 0, N);

    return 0;
}