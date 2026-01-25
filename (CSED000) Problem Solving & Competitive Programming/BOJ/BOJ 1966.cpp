// BOJ 1966: Printer Queue

#include <bits/stdc++.h>

using namespace std;

using ll=long long;

using pqig=priority_queue<int, vector<int>, greater<int>>;
using pqllg=priority_queue<long long, vector<long long>, greater<long long>>;
using mii=map<int, int>;
using umii=unordered_map<int, int>;

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
void print_1D(const vector<TN> &v, int i1, int i2){
    for(int i(i1); i<=i2; ++i){
        cout<<v[i]<<' ';
    }
    cout<<'\n';
}

template <typename TN1, typename TN2>
void print_1D(const vector<pair<TN1, TN2>> &v, int i1, int i2){
    for(int i(i1); i<=i2; ++i){
        cout<<'('<<v[i].first<<", "<<v[i].second<<") ";
    }
    cout<<'\n';
}

template <typename TN>
void print_2D(const vector<vector<TN>> &v, int i1, int i2, int j1, int j2){
    for(int i(i1); i<=i2; ++i){
        for(int j(j1); j<=j2; ++j){
            cout<<v[i][j]<<' ';
        }
        cout<<'\n';
    }
}

// const ll mdls(1000000007LL);
// const int INF(INT_MAX/4);
// const int offset(500000);
// const vector<pair<int, int>> delta{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}, UDLR{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int T; cin>>T;
    while(T--){
        int n, m; cin>>n>>m;
        deque<pair<int, int>> q;
        priority_queue<int> pq;
        for(int i(0); i<n; ++i){
            int job; cin>>job;
            q.push_back({i, job});
            pq.push(job);
        }
        int printed(0);
        while(!q.empty()){
            pair<int, int> cur(q.front()); q.pop_front();
            if(cur.second==pq.top()){
                ++printed;
                pq.pop();
                if(cur.first==m){
                    cout<<printed<<'\n';
                    break;
                }
            } else{
                q.push_back(cur);
            }
        }
    }

    return 0;
}