// Reference: BOJ 11662

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <utility>
#include <tuple>
// #include <limits>
#include <cmath>

using namespace std;

using ll=long long;
using vi=vector<int>;
using vll=vector<long long>;
using vd=vector<double>;
using vb=vector<bool>;
using sti=stack<int>;
using qi=queue<int>;
using dqi=deque<int>;
using dqll=deque<long long>;
using pii=pair<int, int>;
using vvi=vector<vector<int>>;
using vpii=vector<pair<int, int>>;
using vpipii=vector<pair<int, pair<int, int>>>;
using dqpii=deque<pair<int, int>>;
using vvpii=vector<vector<pair<int, int>>>;

#define INT_MAX 2147483647
#define INT_MIN -2147483648
#define LLONG_MAX 9223372036854775807
#define LLONG_MIN -9223372036854775808

// vector = vec = v
// stack = sta = st
// queue = que = q
// deque = dq
// ans = answer
// cur = current
// coordinate = coord
// adjacency matrix = adj_mat

// const int INF=INT_MAX/4; // INF = INFinity

class coord{
    public:
        double x, y;
        coord(double a=-1, double b=-1){
            x=a;
            y=b;
        }
};

class journey{
    public:
        coord from, to;

        coord get_cur_coord(double t){
            coord cur(from.x+(to.x-from.x)*t, from.y+(to.y-from.y)*t);
            return cur;
        }
};

double get_distance(coord P, coord Q){
    return sqrt(pow(P.x-Q.x, 2)+pow(P.y-Q.y, 2));
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    // cout.tie(nullptr);
    
    cout<<fixed;
    cout.precision(10);

    journey Jake, Benji;
    cin>>Jake.from.x>>Jake.from.y>>Jake.to.x>>Jake.to.y>>Benji.from.x>>Benji.from.y>>Benji.to.x>>Benji.to.y;

    vd dist(4);
    vd t={0, -1, -1, 1};

    for(int i=0; i<1e5; i++){
        t[1]=(t[0]*2+t[3])/3;
        t[2]=(t[0]+t[3]*2)/3;
        dist[1]=get_distance(Jake.get_cur_coord(t[1]), Benji.get_cur_coord(t[1]));
        dist[2]=get_distance(Jake.get_cur_coord(t[2]), Benji.get_cur_coord(t[2]));
        if(dist[1]==0 || dist[2]==0){
            cout<<"0";
            return 0;
        }
        else if(dist[1]>=dist[2]){
            t[0]=t[1];
        }
        else{
            t[3]=t[2];
        }
    }
    cout<<get_distance(Jake.get_cur_coord((t[0]+t[3])/2), Benji.get_cur_coord((t[0]+t[3])/2));

    return 0;
}