// BOJ 5405: Fractal Streets(프랙탈 거리)

// #include <bits/stdc++.h>

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <numeric>
#include <utility>
#include <tuple>
#include <climits>
#include <limits>
#include <cmath>
#include <bitset>
#include <functional>
#include <cassert>

using namespace std;

using ll=long long;
using vi=vector<int>;
using vll=vector<long long>;
using vd=vector<double>;
using vb=vector<bool>;
using vstr=vector<string>;
// using sti=stack<int>;
// using stch=stack<char>;
// using qi=queue<int>;
using dqi=deque<int>;
using dqll=deque<long long>;
using pqi=priority_queue<int>;
using pqig=priority_queue<int, vector<int>, greater<int>>;
using pqll=priority_queue<long long>;
using pqllg=priority_queue<long long, vector<long long>, greater<long long>>;
using pii=pair<int, int>;
using ppiii=pair<pair<int, int>, int>;
using pipii=pair<int, pair<int, int>>;
using pci=pair<char, int>;
using vpii=vector<pair<int, int>>;
using vpci=vector<pair<char, int>>;
using vppiii=vector<pair<pair<int, int>, int>>;
using vpipii=vector<pair<int, pair<int, int>>>;
using dqpii=deque<pair<int, int>>;
using vvi=vector<vector<int>>;
using vvll=vector<vector<long long>>;
using vvpii=vector<vector<pair<int, int>>>;

using mii=map<int, int>;

#define elif else if

// const int INF(INT_MAX);
// const ll mod(1000000007); // 10^9+7
// const int offset(500000);

pii Hilbert_curve(int order, int target){
    if(order==1){
        switch(target){
            case 1:
                return {0, 0};
            case 2:
                return {0, 10};
            case 3:
                return {10, 10};
            case 4:
                return {10, 0};
        }
    }
    int div_num((int)pow(4, order-1)), correction(((int)pow(2, order))*5), x_symmetry((((int)pow(2, order))-1)*5), y_symmetry((((int)pow(2, order-1))-1)*5);
    pii p(Hilbert_curve(order-1, ((target-1)%div_num)+1));
    if(target<=div_num) return {p.second, p.first};
    elif(target<=div_num*2) return {p.first, p.second+correction};
    elif(target<=div_num*3) return {p.first+correction, p.second+correction};
    else return {2*x_symmetry-p.second, 2*y_symmetry-p.first};
}

int dist(pii p, pii q){
    return (int)(sqrt(pow(p.first-q.first, 2)+pow(p.second-q.second, 2))+0.5);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cout<<fixed;
    cout.precision(10);

    int T;
    cin>>T;
    while(T--){
        int n, h, o;
        cin>>n>>h>>o;
        cout<<dist(Hilbert_curve(n, h), Hilbert_curve(n, o))<<"\n";
    }
    
    return 0;
}