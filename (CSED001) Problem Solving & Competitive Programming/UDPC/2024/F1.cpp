// BOJ 31716: 현대모비스 자율 주행 테스팅 1

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

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int N, K;
    cin>>N>>K;
    vstr S(2);
    cin>>S[0]>>S[1];

    for(int i(0); i<N; ++i){
        if((S[0][i]=='#' && S[1][i]=='#') || (i+1<N && S[0][i]=='#' && S[1][i+1]=='#') || (i+1<N && S[1][i]=='#' && S[0][i+1]=='#')){
            cout<<-1;
            return 0;
        }
    }
    if(K>1 && ((S[0][N-1]=='#' && S[1][0]=='#') || (S[1][N-1]=='#' && S[0][0]=='#'))){
        cout<<-1;
        return 0;
    }
    
    vi obstacle;
    for(int i(0); i<N; ++i){
        if(S[0][i]=='#'){
            obstacle.push_back(0);
        }
        elif(S[1][i]=='#'){
            obstacle.push_back(1);
        }
    }
    int O((int)obstacle.size());
    if(O==0 || O==1){
        cout<<(ll)((ll)N*(ll)K-1LL);
        return 0;
    }

    ll ans(0);
    for(int i(0); i<O-1; ++i){
        if(obstacle[i]!=obstacle[i+1]){
            ++ans;
        }
    }
    ans*=(ll)K;
    if(obstacle[O-1]!=obstacle[0]){
        ans+=(ll)(K-1);
    }
    ans+=(ll)((ll)N*(ll)K-1LL);
    cout<<ans;
    return 0;
}