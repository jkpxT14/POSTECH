// BOJ 23817: 포항항

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
using vvvvi=vector<vector<vector<vector<int>>>>;

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
void print_1D(const vector<TN> &v, int type, int N){
    for(int i(type); i<N+type; ++i){
        cout<<v[i]<<' ';
    }
    cout<<'\n';
}

template <typename TN1, typename TN2>
void print_1D(const vector<pair<TN1, TN2>> &v, int type, int N){
    for(int i(type); i<N+type; ++i){
        cout<<'('<<v[i].first<<", "<<v[i].second<<") ";
    }
    cout<<'\n';
}

template <typename TN>
void print_2D(const vector<vector<TN>> &v, int type, int M, int N){
    for(int i(type); i<M+type; ++i){
        for(int j(type); j<N+type; ++j){
            cout<<v[i][j]<<' ';
        }
        cout<<'\n';
    }
}

int mod(int x, int modulus){ // modulo
    return ((x%modulus)+modulus)%modulus;
}

// const int INF(INT_MAX/4);
// const ll mod(1000000007);
// const int offset(500000);
const vpii delta{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

int N, M;
vvi board;
vpii restaurant;
int alpha(0);
vvi dist1;
vb vst1;
int ans(INT_MAX);

void bfs(int start){
    dqpii q;
    vvi dist2(N, vi(M, -1));
    vvb vst2(N, vb(M, false));

    q.push_back(restaurant[start]);
    dist2[restaurant[start].first][restaurant[start].second]=0;
    vst2[restaurant[start].first][restaurant[start].second]=true;
    while(!q.empty()){
        pii cur(q.front()); q.pop_front();
        for(int i(0); i<delta.size(); ++i){
            pii nxt(cur+delta[i]);
            if(nxt.first<0 || nxt.first>=N || nxt.second<0 || nxt.second>=M){
                continue;
            }
            if(board[nxt.first][nxt.second]!=-1 && !vst2[nxt.first][nxt.second]){
                q.push_back(nxt);
                dist2[nxt.first][nxt.second]=dist2[cur.first][cur.second]+1;
                vst2[nxt.first][nxt.second]=true;
            }
        }
    }

    for(int i(0); i<=alpha; ++i){
        dist1[start][i]=dist2[restaurant[i].first][restaurant[i].second];
    }
}

void dfs(int cur, int cnt, int sum){
    if(cnt==5){
        ans=min(ans, sum);
        return;
    }
    for(int i(1); i<=alpha; ++i){
        if(dist1[cur][i]!=-1 && !vst1[i]){
            vst1[i]=true;
            dfs(i, cnt+1, sum+dist1[cur][i]);
            vst1[i]=false;
        }
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    cin>>N>>M;
    board.resize(N, vi(M));
    for(int i(0); i<N; ++i){
        for(int j(0); j<M; ++j){
            char c;
            cin>>c;
            if(c=='.'){
                board[i][j]=-2;
            }
            elif(c=='X'){
                board[i][j]=-1;
            }
            elif(c=='S'){
                board[i][j]=0;
                restaurant.insert(restaurant.begin(), {i, j});
            }
            elif(c=='K'){
                board[i][j]=++alpha;
                restaurant.push_back({i, j});
            }
        }
    }

    dist1.resize(alpha+1, vi(alpha+1, -1));
    for(int i(0); i<=alpha; ++i){
        bfs(i);
    }

    vst1.resize(alpha+1, false);
    vst1[0]=true;
    dfs(0, 0, 0);

    cout<<((ans!=INT_MAX)?(ans):(-1));

    return 0;
}