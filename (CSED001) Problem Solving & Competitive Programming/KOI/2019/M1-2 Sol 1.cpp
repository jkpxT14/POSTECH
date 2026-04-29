// BOJ 17611, JUNGOL 3336: 직각다각형

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
pair<TN1, TN2> &operator+=(pair<TN1, TN2> &x, const pair<TN1, TN2> &y){
    x.first+=y.first; x.second+=y.second;
    return x;
}

const int offset(500000);

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int n; cin>>n;
    vector<pair<int, int>> coord(n+1);
    for(int i(0); i<=n-1; ++i){
        int x, y; cin>>x>>y;
        coord[i]={x+offset, y+offset};
    }
    coord[n]=coord[0];
    vector<int> h(2*offset+1, 0), v(2*offset+1, 0);
    for(int i(0); i<=n-1; ++i){
        if(coord[i].first==coord[i+1].first){
            ++h[min(coord[i].second, coord[i+1].second)];
            --h[max(coord[i].second, coord[i+1].second)];
        } elif(coord[i].second==coord[i+1].second){
            ++v[min(coord[i].first, coord[i+1].first)];
            --v[max(coord[i].first, coord[i+1].first)];
        }
    }
    for(int i(1); i<=2*offset; ++i){
        h[i]+=h[i-1];
        v[i]+=v[i-1];
    }
    int ans(-1);
    for(int i(0); i<=2*offset; ++i){
        ans=max({ans, h[i], v[i]});
    }
    cout<<ans;

    return 0;
}