// BOJ 25543: X 만들기

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

struct Point{
    int x, y;

    bool operator <(const Point &other) const {
        if(x!=other.x){
            return x<other.x;
        }
        return y<other.y;
    }
};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int N; cin>>N;
    vector<Point> p(N); for(int i(0); i<=N-1; ++i) cin>>p[i].x>>p[i].y;
    sort(p.begin(), p.end());
    vector<int> NE(N, 1), NW(N, 1), SW(N, 1), SE(N, 1);
    for(int i(N-1); i>=0; --i){
        for(int j(N-1); j>=i+1; --j){
            if(p[j].x>p[i].x && p[j].y>p[i].y){
                NE[i]=max(NE[i], NE[j]+1);
            }
            if(p[j].x>p[i].x && p[j].y<p[i].y){
                SE[i]=max(SE[i], SE[j]+1);
            }
        }
    }
    for(int i(0); i<=N-1; ++i){
        for(int j(0); j<=i-1; ++j){
            if(p[j].x<p[i].x && p[j].y>p[i].y){
                NW[i]=max(NW[i], NW[j]+1);
            }
            if(p[j].x<p[i].x && p[j].y<p[i].y){
                SW[i]=max(SW[i], SW[j]+1);
            }
        }
    }
    int best(0);
    for(int i(0); i<=N-1; ++i){
        if(min({NE[i], NW[i], SW[i], SE[i]})>=2){
            best=max(best, NE[i]+NW[i]+SW[i]+SE[i]-3);
        }
    }
    if(best==0){
        cout<<-1;
    } else{
        cout<<N-best;
    }

    return 0;
}