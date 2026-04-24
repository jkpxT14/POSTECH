// BOJ 33951: 사각형 모험

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

const int INF(4000001);

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int N, M, K; cin>>N>>M>>K;
    vector<vector<char>> board(N+1, vector<char>(M+1));
    for(int i(1); i<=N; ++i){
        for(int j(1); j<=M; ++j){
            cin>>board[i][j];
        }
    }
    vector<vector<int>> DP_min_x(N+1, vector<int>(M+1, INF));
    vector<vector<int>> DP_max_x(N+1, vector<int>(M+1, 0));
    for(int i(1); i<=N; ++i){
        for(int j(1); j<=M; ++j){
            int add(board[i][j]=='A');
            if(i==1 && j==1){
                DP_min_x[i][j]=add;
                DP_max_x[i][j]=add;
            } elif(i==1){
                DP_min_x[i][j]=DP_min_x[i][j-1]+add;
                DP_max_x[i][j]=DP_max_x[i][j-1]+add;
            } elif(j==1){
                DP_min_x[i][j]=DP_min_x[i-1][j]+add;
                DP_max_x[i][j]=DP_max_x[i-1][j]+add;
            } else{
                DP_min_x[i][j]=min(DP_min_x[i-1][j], DP_min_x[i][j-1])+add;
                DP_max_x[i][j]=max(DP_max_x[i-1][j], DP_max_x[i][j-1])+add;
            }
        }
    }
    int min_x(DP_min_x[N][M]);
    int max_x(DP_max_x[N][M]);
    int L(N+M-1);
    while(K--){
        int A, B, C; cin>>A>>B>>C;
        if(A==B){
            cout<<(C==L*A?"YES\n":"NO\n");
        } else{
            int D(A-B);
            int E(C-L*B);
            if(E%D==0 && min_x<=E/D && E/D<=max_x){
                cout<<"YES\n";
            } else{
                cout<<"NO\n";
            }
        }
    }

    return 0;
}