// BOJ 10814: 나이순 정렬

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

struct User{
    int age; string name;
    User()=default;
    User(int a, string n): age(a), name(n) {}
};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int N;
    cin>>N;
    vector<User> user(N);
    for(int i(0); i<N; ++i){
        cin>>user[i].age>>user[i].name;
    }
    stable_sort(user.begin(), user.end(), [](const User &a, const User &b){
        return a.age<b.age;
    });
    for(int i(0); i<N; ++i){
        cout<<user[i].age<<' '<<user[i].name<<'\n';
    }

    return 0;
}