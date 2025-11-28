// BOJ 26124: 조명 배치

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

#define elif else if
const auto strnpos=string::npos;

// const int INF(INT_MAX);
// const ll mod(1000000007); // 10^9+7
// const int offset(500000);
const vpii delta{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

int H, W;
vvi brightness;
vpipii v;
int ans(0);

bool bfs(){
    dqpii q;
    vvb visited(H, vb(W, false));
    for(pipii w: v){
        if(visited[w.second.first][w.second.second]){
            continue;
        }
        ++ans;
        if(w.first==1){
            continue;
        }
        q.push_back(w.second);
        while(!q.empty()){
            pii cur(q.front());
            q.pop_front();
            if(brightness[cur.first][cur.second]==1){
                continue;
            }
            for(pii d: delta){
                pii next{cur.first+d.first, cur.second+d.second};
                if(next.first<0 || next.first>=H || next.second<0 || next.second>=W){
                    continue;
                }
                if(brightness[next.first][next.second]==-1){
                    continue;
                }
                int diff(brightness[cur.first][cur.second]-brightness[next.first][next.second]);
                if(diff==0){
                    continue;
                }
                elif(diff==1){
                    if(!visited[next.first][next.second]){
                        q.push_back(next);
                        visited[next.first][next.second]=true;
                    }
                }
                elif(diff>1){
                    return false;
                }
            }
        }
    }
    return true;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed;
    cout.precision(10);

    cin>>H>>W;
    brightness.resize(H, vi(W));
    for(int i(0); i<H; ++i){
        for(int j(0); j<W; ++j){
            cin>>brightness[i][j];
            if(brightness[i][j]>=1){
                v.push_back({brightness[i][j], {i, j}});
            }
        }
    }
    sort(v.begin(), v.end(), greater<pipii>());
    // cout<<bfs()?ans:-1;
    cout<<(bfs()?ans:-1);
    
    return 0;
}