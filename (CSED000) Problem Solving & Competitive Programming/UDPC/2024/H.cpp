// BOJ 31722: 산수화

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
void print_1D(const vector<TN> &v, int i1, int i2){
    for(int i(i1); i<=i2; ++i){
        cout<<v[i]<<' ';
    }
    cout<<'\n';
}

template <typename TN1, typename TN2>
void print_1D(const vector<pair<TN1, TN2>> &v, int i1, int i2){
    for(int i(i1); i<=i2; ++i){
        cout<<'('<<v[i].first<<", "<<v[i].second<<") ";
    }
    cout<<'\n';
}

template <typename TN>
void print_2D(const vector<vector<TN>> &v, int i1, int i2, int j1, int j2){
    for(int i(i1); i<=i2; ++i){
        for(int j(j1); j<=j2; ++j){
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

    int N, M;
    cin>>N>>M;
    vvc lp(N+1, vc(M+1)); // lp = landscape painting
    for(int i(1); i<=N; ++i){
        for(int j(1); j<=M; ++j){
            cin>>lp[i][j];
        }
    }

    vvi MT(N+1, vi(M+1)), LK(N+1, vi(M+1)); // MT = MounTain, LK = LaKe
    for(int i(N); i>=1; --i){
        for(int j(1); j<=M; ++j){
            if(lp[i][j]=='#'){
                MT[i][j]=min(((i+1<=N && j-1>=1)?MT[i+1][j-1]:0), min((i+1<=N?MT[i+1][j]:0), ((i+1<=N && j+1<=M)?MT[i+1][j+1]:0)))+1;
            } else{
                MT[i][j]=0;
            }
        }
    }
    for(int i(1); i<=N; ++i){
        for(int j(1); j<=M; ++j){
            if(lp[i][j]=='.'){
                LK[i][j]=min(((i-1>=1 && j-1>=1)?LK[i-1][j-1]:0), min((i-1>=1?LK[i-1][j]:0), (j-1>=1?LK[i][j-1]:0)))+1;
            } else{
                LK[i][j]=0;
            }
        }
    }

    vi A(N+2, 0), B(N+2, 0);
    for(int i(1); i<=N; ++i){
        for(int j(1); j<=M; ++j){
            ++A[0]; --A[MT[i][j]+1];
            ++B[0]; --B[LK[i][j]+1];
        }
    }
    for(int i(1); i<=N+1; ++i){
        A[i]+=A[i-1];
        B[i]+=B[i-1];
    }
    print_1D(A, 1, N);
    print_1D(B, 1, N);

    return 0;
}