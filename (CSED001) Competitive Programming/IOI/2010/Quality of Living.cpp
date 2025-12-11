// BOJ 10227: Quality of Living

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
// const vpii delta{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int R, C, H, W;
    cin>>R>>C>>H>>W;
    vvi Q(R, vi(C));
    for(int i(0); i<R; ++i){
        for(int j(0); j<C; ++j){
            cin>>Q[i][j];
        }
    }

    int left(1), right(R*C);
    while(left<=right){
        int mid((left+right)>>1);

        vvi check(R, vi(C));
        for(int i(0); i<R; ++i){
            for(int j(0); j<C; ++j){
                if(Q[i][j]<mid){
                    check[i][j]=-1;
                }
                elif(Q[i][j]==mid){
                    check[i][j]=0;
                }
                else{
                    check[i][j]=1;
                }
            }
        }

        vvi P(R, vi(C));
        P[0][0]=check[0][0];
        for(int i(1); i<R; ++i){
            P[i][0]=check[i][0]+P[i-1][0];
        }
        for(int j(1); j<C; ++j){
            P[0][j]=check[0][j]+P[0][j-1];
        }
        for(int i(1); i<R; ++i){
            for(int j(1); j<C; ++j){
                P[i][j]=check[i][j]+P[i][j-1]+P[i-1][j]-P[i-1][j-1];
            }
        }

        int flag(INT_MAX);
        flag=min(flag, P[H-1][W-1]);
        for(int i(H); i<R; ++i){
            flag=min(flag, P[i][W-1]-P[i-H][W-1]);
        }
        for(int j(W); j<C; ++j){
            flag=min(flag, P[H-1][j]-P[H-1][j-W]);
        }
        for(int i(H); i<R; ++i){
            for(int j(W); j<C; ++j){
                flag=min(flag, P[i][j]-P[i-H][j]-P[i][j-W]+P[i-H][j-W]);
            }
        }
        if(flag>0){
            left=mid+1;
        }
        elif(flag==0){
            cout<<mid;
            return 0;
        }
        else{
            right=mid-1;
        }
    }

    return 0;
}