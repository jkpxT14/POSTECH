// BOJ 2629: 양팔저울

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

int M;
vi counterweight;
int sum(0);
vvb dp;

void f(int cnt, int weight){
    if(dp[cnt][weight]){
        return;
    }
    dp[cnt][weight]=true;
    if(cnt==M){
        return;
    }
    f(cnt+1, weight);
    f(cnt+1, weight+counterweight[cnt]);
    f(cnt+1, abs(weight-counterweight[cnt]));
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    cin>>M;
    counterweight.resize(M);
    for(int i(0); i<M; ++i){
        cin>>counterweight[i];
        sum+=counterweight[i];
    }
    dp.resize(M+1, vb(sum+1, false));
    f(0, 0);
    int N;
    cin>>N;
    for(int i(0); i<N; ++i){
        int target;
        cin>>target;
        if(target>sum || !dp[M][target]){
            cout<<"N ";
        }
        else{
            cout<<"Y ";
        }
    }

    return 0;
}