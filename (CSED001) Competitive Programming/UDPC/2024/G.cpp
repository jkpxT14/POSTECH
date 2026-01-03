// BOJ 31720: Twitch Plays Pokemon

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
using umii=unordered_map<int, int>;

using vvvb=vector<vector<vector<bool>>>;
using vvvvb=vector<vector<vector<vector<bool>>>>;
using vvvvi=vector<vector<vector<vector<int>>>>;
using vvstr=vector<vector<string>>;
using vpvii=vector<pair<vector<int>, int>>;
using tiiii=tuple<int, int, int, int>;
using dqtiiii=deque<tuple<int, int, int, int>>;

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
// const ll mdls(1000000007LL);
// const int offset(500000);
// const vpii delta{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
const vpii UDLR{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int N;
    cin>>N;
    vvc maze(N, vc(N));
    pii start, end;
    for(int i(0); i<N; ++i){
        for(int j(0); j<N; ++j){
            cin>>maze[i][j];
            if(maze[i][j]=='S'){
                start={i, j};
            }
            elif(maze[i][j]=='E'){
                end={i, j};
            }
        }
    }
    string cD1, cP1;
    cin>>cD1>>cP1;
    cD1.insert(0, "N"); cP1.insert(0, "N");
    vi cD2, cP2;
    for(int i(0); i<cD1.size(); ++i){
        if(cD1[i]=='N'){
            cD2.push_back(-1);
        }
        elif(cD1[i]=='U'){
            cD2.push_back(0);
        }
        elif(cD1[i]=='D'){
            cD2.push_back(1);
        }
        elif(cD1[i]=='L'){
            cD2.push_back(2);
        }
        elif(cD1[i]=='R'){
            cD2.push_back(3);
        }
    }
    for(int i(0); i<cP1.size(); ++i){
        if(cP1[i]=='N'){
            cP2.push_back(-1);
        }
        elif(cP1[i]=='U'){
            cP2.push_back(0);
        }
        elif(cP1[i]=='D'){
            cP2.push_back(1);
        }
        elif(cP1[i]=='L'){
            cP2.push_back(2);
        }
        elif(cP1[i]=='R'){
            cP2.push_back(3);
        }
    }

    dqtiiii q;
    vvvvb vst(cD2.size(), vvvb(cP2.size(), vvb(N, vb(N, false))));
    q.push_back({0, 0, start.first, start.second});
    vst[0][0][start.first][start.second]=true;
    while(!q.empty()){
        tiiii cur(q.front());
        q.pop_front();
        if(get<2>(cur)==end.first && get<3>(cur)==end.second){
            cout<<get<0>(cur)+get<1>(cur);
            return 0;
        }
        tiiii nxt;
        if(get<0>(cur)<cD2.size()-1){
            nxt={get<0>(cur)+1, get<1>(cur), get<2>(cur)+UDLR[cD2[get<0>(cur)+1]].first, get<3>(cur)+UDLR[cD2[get<0>(cur)+1]].second};
            if(!(0<=get<2>(nxt) && get<2>(nxt)<N && 0<=get<3>(nxt) && get<3>(nxt)<N && maze[get<2>(nxt)][get<3>(nxt)]!='#')){
                nxt={get<0>(cur)+1, get<1>(cur), get<2>(cur), get<3>(cur)};
                if(!vst[get<0>(nxt)][get<1>(nxt)][get<2>(nxt)][get<3>(nxt)]){
                    q.push_back(nxt);
                    vst[get<0>(nxt)][get<1>(nxt)][get<2>(nxt)][get<3>(nxt)]=true;
                }
            }
            elif(!vst[get<0>(nxt)][get<1>(nxt)][get<2>(nxt)][get<3>(nxt)]){
                q.push_back(nxt);
                vst[get<0>(nxt)][get<1>(nxt)][get<2>(nxt)][get<3>(nxt)]=true;
            }
        }
        if(get<1>(cur)<cP2.size()-1){
            nxt={get<0>(cur), get<1>(cur)+1, get<2>(cur)+UDLR[cP2[get<1>(cur)+1]].first, get<3>(cur)+UDLR[cP2[get<1>(cur)+1]].second};
            if(!(0<=get<2>(nxt) && get<2>(nxt)<N && 0<=get<3>(nxt) && get<3>(nxt)<N && maze[get<2>(nxt)][get<3>(nxt)]!='#')){
                nxt={get<0>(cur), get<1>(cur)+1, get<2>(cur), get<3>(cur)};
                if(!vst[get<0>(nxt)][get<1>(nxt)][get<2>(nxt)][get<3>(nxt)]){
                    q.push_back(nxt);
                    vst[get<0>(nxt)][get<1>(nxt)][get<2>(nxt)][get<3>(nxt)]=true;
                }
            }
            elif(!vst[get<0>(nxt)][get<1>(nxt)][get<2>(nxt)][get<3>(nxt)]){
                q.push_back(nxt);
                vst[get<0>(nxt)][get<1>(nxt)][get<2>(nxt)][get<3>(nxt)]=true;
            }
        }
    }
    cout<<-1;
    return 0;
}