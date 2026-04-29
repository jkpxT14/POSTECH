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

// const ll mdls(1000000007LL);
// const int INF(INT_MAX/4);
// const int offset(500000);
// const array<pair<int, int>, 4> delta{{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};

int f(vector<pair<int, int>> &L){
    sort(L.begin(), L.end());
    priority_queue<int, vector<int>, greater<int>> pq;
    int ret(-1);
    for(int i(0); i<=L.size()-1; ++i){
        while(!pq.empty() && pq.top()<=L[i].first){
            pq.pop();
        }
        pq.push(L[i].second);
        ret=max(ret, int(pq.size()));
    }
    return ret;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int n; cin>>n;
    vector<pair<int, int>> coord(n+1);
    for(int i(0); i<=n-1; ++i){
        cin>>coord[i].first>>coord[i].second;
    }
    coord[n]=coord[0];
    vector<pair<int, int>> H, V;
    for(int i(0); i<=n-1; ++i){
        if(coord[i].first==coord[i+1].first){
            H.push_back({min(coord[i].second, coord[i+1].second), max(coord[i].second, coord[i+1].second)});
        } elif(coord[i].second==coord[i+1].second){
            V.push_back({min(coord[i].first, coord[i+1].first), max(coord[i].first, coord[i+1].first)});
        }
    }
    int h(f(H)), v(f(V));
    cout<<max(h, v);

    return 0;
}