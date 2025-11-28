// BOJ 31713: 행운을 빌어요

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

int A, B;

void f(int X, int Y){
    // cout<<X<<' '<<Y<<'\n';
    if(3*X<=Y && Y<=4*X){
        cout<<X-A+Y-B<<'\n';
    }
    elif(Y<3*X){
        f(X, 3*X);
    }
    elif(Y>4*X){
        f(X+(((Y-4*X)%4==0)?((Y-4*X)/4):((Y-4*X)/4+1)), Y);
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed;
    cout.precision(10);
    
    int T;
    cin>>T;
    while(T--){
        cin>>A>>B;
        f(A, B);
    }
    
    return 0;
}