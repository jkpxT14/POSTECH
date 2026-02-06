// BOJ 1006: 습격자 초라기

#include <bits/stdc++.h>

using namespace std;

using ll=long long;

using pqig=priority_queue<int, vector<int>, greater<int>>;
using pqllg=priority_queue<long long, vector<long long>, greater<long long>>;

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

// const ll mdls(1000000007LL);
// const int INF(INT_MAX/4);
// const int offset(500000);
// const array<pair<int, int>, 4> delta{{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};

int N, W;
array<vector<int>, 2> enemy;
array<vector<int>, 3> dp;

void compute_dp(){
    for(int i(1); i<=N-1; ++i){
        array<int, 3> two{{((enemy[0][i-1]+enemy[0][i]<=W)?1:2), ((enemy[1][i-1]+enemy[1][i]<=W)?1:2), ((enemy[0][i]+enemy[1][i]<=W)?1:2)}};
        dp[0][i]=min(dp[1][i-1]+two[0], dp[2][i-1]+1);
        dp[1][i]=min(dp[0][i-1]+two[1], dp[2][i-1]+1);
        dp[2][i]=min(dp[0][i]+1, min(dp[1][i]+1, min((i>=2?dp[2][i-2]:0)+two[0]+two[1], dp[2][i-1]+two[2])));
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int T; cin>>T;
    while(T--){
        cin>>N>>W;
        enemy.fill(vector<int>(N));
        for(int i(0); i<=1; ++i){
            for(int j(0); j<=N-1; ++j){
                cin>>enemy[i][j];
            }
        }

        if(N==1){
            cout<<((enemy[0][0]+enemy[1][0]<=W)?1:2)<<'\n';
            continue;
        }

        array<int, 2> temp{{enemy[0][0], enemy[1][0]}};
        dp.fill(vector<int>(N, 2*N));
        int ans(2*N);

        dp[0][0]=1; dp[1][0]=1; dp[2][0]=((enemy[0][0]+enemy[1][0]<=W)?1:2);
        compute_dp();
        ans=min(ans, dp[2][N-1]);
        dp.fill(vector<int>(N, 2*N));

        if(enemy[0][0]+enemy[0][N-1]<=W){
            dp[0][0]=1; dp[1][0]=1; dp[2][0]=2;
            enemy[0][0]=W+1;
            compute_dp();
            ans=min(ans, dp[1][N-1]);
            enemy[0][0]=temp[0];
            dp.fill(vector<int>(N, 2*N));
        }

        if(enemy[1][0]+enemy[1][N-1]<=W){
            dp[0][0]=1; dp[1][0]=1; dp[2][0]=2;
            enemy[1][0]=W+1;
            compute_dp();
            ans=min(ans, dp[0][N-1]);
            enemy[1][0]=temp[1];
            dp.fill(vector<int>(N, 2*N));
        }

        if(enemy[0][0]+enemy[0][N-1]<=W && enemy[1][0]+enemy[1][N-1]<=W){
            dp[0][0]=1; dp[1][0]=1; dp[2][0]=2;
            enemy[0][0]=W+1; enemy[1][0]=W+1;
            compute_dp();
            ans=min(ans, dp[2][N-2]);
            enemy[0][0]=temp[0]; enemy[1][0]=temp[1];
            dp.fill(vector<int>(N, 2*N));
        }

        cout<<ans<<'\n';
    }

    return 0;
}