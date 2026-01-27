// BOJ 1764: 듣보잡

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
// const vector<pair<int, int>> delta{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}, UDLR{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int N, M; cin>>N>>M;
    unordered_map<string, int> um;
    set<string> ans;
    for(int i(0); i<=N+M-1; ++i){
        string name; cin>>name;
        if(++um[name]>=2){
            ans.insert(name);
        }
    }
    cout<<ans.size()<<'\n';
    for(string s: ans){
        cout<<s<<'\n';
    }

    return 0;
}