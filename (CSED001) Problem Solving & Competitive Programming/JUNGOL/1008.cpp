// JUNGOL 1008: 경로찾기(find route)

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

const int INF(INT_MAX/2);
const array<pair<int, int>, 4> delta{{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int n, t; cin>>n>>t;
    vector<vector<int>> board(n, vector<int>(n));
    pair<int, int> src, dst;
    for(int i(0); i<=n-1; ++i){
        for(int j(0); j<=n-1; ++j){
            cin>>board[i][j];
            if(board[i][j]==0){
                board[i][j]=INF;
            } elif(board[i][j]==-1){
                src={i, j};
                board[i][j]==0;
            } elif(board[i][j]==-2){
                dst={i, j};
                board[i][j]=0;
            }
        }
    }
    vector<vector<int>> cmi(n, vector<int>(n, INF)), nmi(n, vector<int>(n, INF)); // cmi=cur_min_insolation, nmi=nxt_min_insolation
    cmi[src.first][src.second]=0;
    int ans(INF);
    for(int time(1); time<=t; ++time){
        bool updated(false);
        for(int x(0); x<=n-1; ++x){
            for(int y(0); y<=n-1; ++y){
                if(cmi[x][y]==INF){
                    continue;
                }
                for(pair<int, int> dt: delta){
                    int nx(x+dt.first), ny(y+dt.second);
                    if(nx<=-1 || nx>=n || ny<=-1 || ny>=n){
                        continue;
                    }
                    if(board[nx][ny]==INF){
                        continue;
                    }
                    if(nmi[nx][ny]>cmi[x][y]+board[nx][ny]){
                        nmi[nx][ny]=cmi[x][y]+board[nx][ny];
                        updated=true;
                    }
                }
            }
        }
        if(!updated){
            break;
        }
        ans=min(ans, nmi[dst.first][dst.second]);
        swap(cmi, nmi);
        nmi.assign(n, vector<int>(n, INF));
    }
    if(ans==INF){
        cout<<-1;
    } else{
        cout<<ans;
    }

    return 0;
}