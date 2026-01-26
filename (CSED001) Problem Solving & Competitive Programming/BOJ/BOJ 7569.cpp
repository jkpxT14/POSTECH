// BOJ 7569: 토마토

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
    x.first+=y.first; x.second+=y.second;
    return x;
}

struct coordinate{
    int x, y, z;

    friend coordinate operator+(const coordinate &p, const coordinate &q){
        return {p.x+q.x, p.y+q.y, p.z+q.z};
    }
    coordinate & operator+=(const coordinate &p){
        this->x+=p.x; this->y+=p.y; this->z+=p.z;
        return *this;
    }
    friend coordinate operator-(const coordinate &p, const coordinate &q){
        return {p.x-q.x, p.y-q.y, p.z-q.z};
    }
    coordinate & operator-=(const coordinate &p){
        this->x-=p.x; this->y-=p.y; this->z-=p.z;
        return *this;
    }
};

// const ll mdls(1000000007LL);
// const int INF(INT_MAX/4);
// const int offset(500000);
const vector<coordinate> delta{{0, 0, 1}, {0, 0, -1}, {0, 1, 0}, {0, -1, 0}, {1, 0, 0}, {-1, 0, 0}};

int M, N, H;
vector<vector<vector<int>>> tmt;
deque<coordinate> q;

void BFS(){
    while(!q.empty()){
        coordinate cur(q.front()); q.pop_front();
        for(coordinate d: delta){
            coordinate nxt(cur+d);
            if(0<=nxt.x && nxt.x<=M-1 && 0<=nxt.y && nxt.y<=N-1 && 0<=nxt.z && nxt.z<=H-1 && tmt[nxt.x][nxt.y][nxt.z]==0){
                tmt[nxt.x][nxt.y][nxt.z]=tmt[cur.x][cur.y][cur.z]+1;
                q.push_back(nxt);
            }
        }
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    cin>>M>>N>>H;
    tmt.resize(M, vector<vector<int>>(N, vector<int>(H)));
    for(int k(0); k<=H-1; ++k){
        for(int j(0); j<=N-1; ++j){
            for(int i(0); i<=M-1; ++i){
                cin>>tmt[i][j][k];
                if(tmt[i][j][k]==1){
                    q.push_back({i, j, k});
                }
            }
        }
    }
    BFS();
    int ans(-1);
    for(int i(0); i<=M-1; ++i){
        for(int j(0); j<=N-1; ++j){
            for(int k(0); k<=H-1; ++k){
                if(!tmt[i][j][k]){
                    cout<<-1;
                    return 0;
                }
                ans=max(ans, tmt[i][j][k]-1);
            }
        }
    }
    cout<<ans;
    return 0;
}