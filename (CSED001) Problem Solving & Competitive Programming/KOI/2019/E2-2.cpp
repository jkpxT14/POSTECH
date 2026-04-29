// BOJ 17615, JUNGOL 3427: 볼 모으기(balls)

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

    int N; cin>>N;
    string ball; cin>>ball;
    int cnt_R(0), cnt_B(0);
    for(char c: ball){
        if(c=='R'){
            ++cnt_R;
        } elif(c=='B'){
            ++cnt_B;
        }
    }
    int ans(min(cnt_R, cnt_B));
    vector<pair<char, int>> block;
    char cur_color(ball[0]);
    int cur_idx(0);
    bool flag(false);
    while(true){
        int consecutive(0);
        while(ball[cur_idx]==cur_color && cur_idx<=N-1){
            ++consecutive;
            ++cur_idx;
            if(cur_idx==N){
                flag=true;
            }
        }
        block.push_back({cur_color, consecutive});
        if(flag){
            break;
        }
        cur_color=ball[cur_idx];
    }
    if(block[0].first=='R'){
        ans=min(ans, cnt_R-block[0].second);
    }
    if(block[0].first=='B'){
        ans=min(ans, cnt_B-block[0].second);
    }
    if(block[block.size()-1].first=='R'){
        ans=min(ans, cnt_R-block[block.size()-1].second);
    }
    if(block[block.size()-1].first=='B'){
        ans=min(ans, cnt_B-block[block.size()-1].second);
    }
    cout<<ans;

    return 0;
}