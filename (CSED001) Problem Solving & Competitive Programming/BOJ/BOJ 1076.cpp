// BOJ 1076: 저항

#include <bits/stdc++.h>

using namespace std;

using ll=long long;

using pqig=priority_queue<int, vector<int>, greater<int>>;
using pqllg=priority_queue<long long, vector<long long>, greater<long long>>;

#define elif else if

const auto strnpos=string::npos;

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

    auto convert = [](string s) -> ll {
        if(s=="black"){
            return 0LL;
        } elif(s=="brown"){
            return 1LL;
        } elif(s=="red"){
            return 2LL;
        } elif(s=="orange"){
            return 3LL;
        } elif(s=="yellow"){
            return 4LL;
        } elif(s=="green"){
            return 5LL;
        } elif(s=="blue"){
            return 6LL;
        } elif(s=="violet"){
            return 7LL;
        } elif(s=="grey"){
            return 8LL;
        } elif(s=="white"){
            return 9LL;
        } else{
            return -1LL;
        }
    };
    ll ans(0);
    string temp;
    cin>>temp; ans+=10LL*convert(temp);
    cin>>temp; ans+=convert(temp);
    cin>>temp; ans*=ll(pow(10, convert(temp)));
    cout<<ans;

    return 0;
}