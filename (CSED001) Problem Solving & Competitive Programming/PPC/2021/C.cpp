// BOJ 23815: 똥게임

#include <bits/stdc++.h>

using namespace std;

using ll=long long;
using ull=unsigned long long;

#define elif else if

const auto strnpos(string::npos);

template <typename TN1, typename TN2>
pair<TN1, TN2> operator+(const pair<TN1, TN2> &x, const pair<TN1, TN2> &y){
    return {x.first+y.first, x.second+y.second};
}

template <typename TN1, typename TN2>
pair<TN1, TN2> & operator+=(pair<TN1, TN2> &x, const pair<TN1, TN2> &y){
    x.first+=y.first; x.second+=y.second;
    return x;
}

// const ll mdls(1000000007LL);
// const int INF(INT_MAX/4);
// const int offset(500000);
// const array<pair<int, int>, 4> delta{{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    auto apply_choice = [](int cur, char op, int x) -> int {
        if(cur==-1){
            return -1;
        }
        int nxt(cur);
        if(op=='+'){
            nxt+=x;
        } elif(op=='-'){
            nxt-=x;
        } elif(op=='*'){
            nxt*=x;
        } elif(op=='/'){
            nxt/=x;
        }
        return (nxt<=0?-1:nxt);
    };
    auto apply_best_choice = [&apply_choice](int cur, char op1, int x1, char op2, int x2) -> int {
        if(cur==-1){
            return -1;
        }
        int nxt(max(apply_choice(cur, op1, x1), apply_choice(cur, op2, x2)));
        return (nxt<=0?-1:nxt);
    };

    int N; cin>>N;
    int skipped(-1), unskipped(1);
    while(N--){
        string s1, s2; cin>>s1>>s2;
        char op1(s1[0]), op2(s2[0]);
        int x1(s1[1]-'0'), x2(s2[1]-'0');
        skipped=max(apply_best_choice(skipped, op1, x1, op2, x2), unskipped);
        unskipped=apply_best_choice(unskipped, op1, x1, op2, x2);
    }
    int ans(max(skipped, unskipped));
    if(ans==-1){
        cout<<"ddong game";
    } else{
        cout<<ans;
    }

    return 0;
}