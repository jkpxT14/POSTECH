// BOJ 33961: 체스평평설

#include <bits/stdc++.h>

using namespace std;

using ll=long long;
using ull=unsigned long long;

#define elif else if

const auto strnpos(string::npos);

template <typename TN1, typename TN2>
pair<TN1, TN2> operator +(const pair<TN1, TN2> &x, const pair<TN1, TN2> &y){
    return {x.first+y.first, x.second+y.second};
}

template <typename TN1, typename TN2>
pair<TN1, TN2> &operator +=(pair<TN1, TN2> &x, const pair<TN1, TN2> &y){
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

    int M; cin>>M;
    if(M<=2){
        cout<<"NO";
        return 0;
    }
    vector<pair<int, int>> ans;
    auto add3=[&ans](int c){
        ans.push_back({1, c});
        ans.push_back({2, c+1});
        ans.push_back({1, c+2});
        ans.push_back({2, c});
        ans.push_back({1, c+1});
        ans.push_back({2, c+2});
    };
    auto add4=[&ans](int c){
        ans.push_back({1, c});
        ans.push_back({2, c+1});
        ans.push_back({1, c+3});
        ans.push_back({2, c+2});
        ans.push_back({1, c+1});
        ans.push_back({2, c+3});
        ans.push_back({1, c+2});
        ans.push_back({2, c});
    };
    auto add5=[&ans](int c){
        ans.push_back({1, c});
        ans.push_back({2, c+1});
        ans.push_back({1, c+3});
        ans.push_back({2, c+4});
        ans.push_back({1, c+2});
        ans.push_back({2, c});
        ans.push_back({1, c+1});
        ans.push_back({2, c+2});
        ans.push_back({1, c+4});
        ans.push_back({2, c+3});
    };
    if(M%3==0){
        for(int c(1); c<=M-2; c+=3){
            add3(c);
        }
    } elif(M%3==1){
        for(int c(1); c<=M-6; c+=3){
            add3(c);
        }
        add4(M-3);
    } elif(M%3==2){
        for(int c(1); c<=M-7; c+=3){
            add3(c);
        }
        add5(M-4);
    }
    assert(int(ans.size())==2*M);
    cout<<"YES\n";
    for(int i(0); i<=2*M-1; ++i){
        cout<<ans[i].first<<' '<<ans[i].second<<'\n';
    }
    return 0;
}