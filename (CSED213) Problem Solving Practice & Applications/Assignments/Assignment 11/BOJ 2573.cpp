// BOJ 2573: 빙산

#include <bits/stdc++.h>

using namespace std;

using ll=long long;
using pii=pair<int, int>;
using pll=pair<long long, long long>;
using pci=pair<char, int>;
using pipii=pair<int, pair<int, int>>;
using ppiii=pair<pair<int, int>, int>;
using vi=vector<int>;
using vll=vector<long long>;
using vd=vector<double>;
using vb=vector<bool>;
using vc=vector<char>;
using vstr=vector<string>;
using vpii=vector<pair<int, int>>;
using vpll=vector<pair<long long, long long>>;
using vpci=vector<pair<char, int>>;
using vpipii=vector<pair<int, pair<int, int>>>;
using vppiii=vector<pair<pair<int, int>, int>>;
using vvi=vector<vector<int>>;
using vvll=vector<vector<long long>>;
using vvb=vector<vector<bool>>;
using vvc=vector<vector<char>>;
using vvpii=vector<vector<pair<int, int>>>;
using vvvi=vector<vector<vector<int>>>;
using vvvll=vector<vector<vector<long long>>>;
using dqi=deque<int>;
using dqll=deque<long long>;
using dqpii=deque<pair<int, int>>;
using pqi=priority_queue<int>;
using pqig=priority_queue<int, vector<int>, greater<int>>;
using pqll=priority_queue<long long>;
using pqllg=priority_queue<long long, vector<long long>, greater<long long>>;
using mii=map<int, int>;

using vvvb=vector<vector<vector<bool>>>;
using vvvvb=vector<vector<vector<vector<bool>>>>;

#define elif else if

const auto strnpos=string::npos;

// const int INF(INT_MAX/4);
// const ll mod(1000000007);
// const int offset(500000);
const vpii delta{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

void print_2D(const vvi &v, int type, int M, int N){
    for(int i(type); i<M+type; ++i){
        for(int j(type); j<N+type; ++j){
            cout<<v[i][j]<<' ';
        }
        cout<<'\n';
    }
}

int N, M;
vvi iceberg, sea;
vvb vst;
int max_h(0);

void dfs(int x, int y){
    vst[x][y]=true;
    for(int i(0); i<4; ++i){
        int nx(x+delta[i].first), ny(y+delta[i].second);
        if(0<=nx && nx<N && 0<=ny && ny<M && iceberg[nx][ny] && !vst[nx][ny]){
            dfs(nx, ny);
        }
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    cin>>N>>M;
    iceberg.resize(N, vi(M));
    sea.resize(N, vi(M, 0));
    vst.resize(N, vb(M, false));
    for(int i(0); i<N; ++i){
        for(int j(0); j<M; ++j){
            cin>>iceberg[i][j];
            max_h=max(max_h, iceberg[i][j]);
        }
    }
    for(int i(0); i<N; ++i){
        for(int j(0); j<M; ++j){
            if(!iceberg[i][j]){
                for(int k(0); k<4; ++k){
                    int ni(i+delta[k].first), nj(j+delta[k].second);
                    if(0<=ni && ni<N && 0<=nj && nj<M){
                        ++sea[ni][nj];
                    }
                }
            }
        }
    }

    int t(0);
    while(max_h>0){
        vst.assign(N, vb(M, false));
        bool flag(false);
        for(int i(0); i<N; ++i){
            for(int j(0); j<M; ++j){
                if(iceberg[i][j] && !vst[i][j]){
                    if(!flag){
                        dfs(i, j);
                        flag=true;
                    }
                    else{
                        cout<<t;
                        return 0;
                    }
                }
            }
        }
        for(int i(0); i<N; ++i){
            for(int j(0); j<M; ++j){
                iceberg[i][j]-=sea[i][j];
                if(iceberg[i][j]<0){
                    iceberg[i][j]=0;
                }
            }
        }
        sea.assign(N, vi(M, 0));
        for(int i(0); i<N; ++i){
            for(int j(0); j<M; ++j){
                if(!iceberg[i][j]){
                    for(int k(0); k<4; ++k){
                        int ni(i+delta[k].first), nj(j+delta[k].second);
                        if(0<=ni && ni<N && 0<=nj && nj<M){
                            ++sea[ni][nj];
                        }
                    }
                }
            }
        }
        max_h=0;
        for(int i(0); i<N; ++i){
            for(int j(0); j<M; ++j){
                max_h=max(max_h, iceberg[i][j]);
            }
        }
        ++t;
    }
    cout<<0;

    return 0;
}