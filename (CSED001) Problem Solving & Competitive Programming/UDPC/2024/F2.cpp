// BOJ 31717: 현대모비스 자율 주행 테스팅 2

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

    int M, K;
    cin>>M>>K;
    vi A(K);
    for(int i(0); i<K; ++i){
        cin>>A[i]; --A[i];
    }
    vvstr S(M, vstr(2));
    vi N(M);
    for(int i(0); i<M; ++i){
        cin>>S[i][0]>>S[i][1];
        N[i]=S[i][0].size();
    }

    vb ok(M, true);
    for(int i(0); i<M; ++i){
        for(int j(0); j<N[i]; ++j){
            if((S[i][0][j]=='#' && S[i][1][j]=='#') || (j+1<N[i] && S[i][0][j]=='#' && S[i][1][j+1]=='#') || (j+1<N[i] && S[i][0][j+1]=='#' && S[i][1][j]=='#')){
                ok[i]=false;
            }
        }
    }
    for(int i(0); i<K; ++i){
        if(!ok[A[i]]){
            cout<<-1;
            return 0;
        }
    }
    for(int i(0); i<K-1; ++i){
        if((S[A[i]][0][N[A[i]]-1]=='#' && S[A[i+1]][1][0]=='#') || (S[A[i]][1][N[A[i]]-1]=='#' && S[A[i+1]][0][0]=='#')){
            cout<<-1;
            return 0;
        }
    }

    vpvii ob(M, {vi(0), 0}); // ob = obstacle
    for(int i(0); i<M; ++i){
        for(int j(0); j<N[i]; ++j){
            if(S[i][0][j]=='#'){
                if(!ob[i].first.empty() && ob[i].first.back()==1){
                    ++ob[i].second;
                }
                ob[i].first.push_back(0);
            }
            elif(S[i][1][j]=='#'){
                if(!ob[i].first.empty() && ob[i].first.back()==0){
                    ++ob[i].second;
                }
                ob[i].first.push_back(1);
            }
        }
    }

    ll ans(0);
    int prv(-1);
    for(int i(0); i<K; ++i){
        ans+=(ll)ob[A[i]].second;
        if(prv==-1 && !ob[A[i]].first.empty()){
            prv=ob[A[i]].first.back();
        }
        elif(!ob[A[i]].first.empty()){
            if(prv!=ob[A[i]].first.front()){
                ++ans;
            }
            prv=ob[A[i]].first.back();
        }
    }
    for(int i(0); i<K; ++i){
        ans+=(ll)N[A[i]];
    }
    --ans;
    cout<<ans;
    return 0;
}