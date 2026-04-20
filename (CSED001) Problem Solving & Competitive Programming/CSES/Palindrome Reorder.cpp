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

    string s1; cin>>s1;
    vector<int> cnt(26, 0);
    for(int i(0); i<=s1.size()-1; ++i){
        ++cnt[s1[i]-'A'];
    }
    int odd_idx(-1);
    int no_solution(0);
    for(int i(0); i<=25; ++i){
        if(cnt[i]%2==1){
            if(no_solution==0){
                odd_idx=i;
                ++no_solution;
            } else{
                cout<<"NO SOLUTION";
                return 0;
            }
        }
    }
    string s2((odd_idx==-1?"":string(cnt[odd_idx], 'A'+odd_idx)));
    for(int i(0); i<=25; ++i){
        if(i!=odd_idx){
            s2.insert(0, cnt[i]/2, 'A'+i);
            s2.append(cnt[i]/2, 'A'+i);
        }
    }
    cout<<s2;
    return 0;
}