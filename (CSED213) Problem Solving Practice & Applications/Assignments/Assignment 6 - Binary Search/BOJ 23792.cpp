// BOJ 23792: K번째 음식 찾기 2

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

#define elif else if

// const int INF(INT_MAX/2);
// const ll mod(1000000007); // 10^9+7
// const int offset(500000);

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cout<<fixed;
    cout.precision(10);

    int N;
    cin>>N;
    vll A(N+1), B(N+1), C(N+1);
    for(int i=1; i<=N; ++i) cin>>A[i];
    for(int i=1; i<=N; ++i) cin>>B[i];
    for(int i=1; i<=N; ++i) cin>>C[i];
    vvll D(4); D[1]=A; D[2]=B; D[3]=C;
    int Q;
    cin>>Q;
    while(Q--){
        int x, y, z, k;
        cin>>x>>y>>z>>k;
        vi w(4); w[1]=x; w[2]=y; w[3]=z;
        bool found(false);
        for(int i=1; i<=3; ++i){
            int left(1), right(w[i]);
            while(left<=right){
                int mid((left+right)>>1);
                int ranking(mid);
                for(int j=1; j<=3; ++j){
                    if(i==j) continue;
                    int left2(1), right2(w[j]);
                    while(left2<=right2){
                        int mid2((left2+right2)>>1);
                        (D[j][mid2]<D[i][mid])?(left2=mid2+1):(right2=mid2-1);
                    }
                    ranking+=right2;
                }
                if(ranking==k){
                    cout<<i<<" "<<mid<<"\n";
                    found=true;
                    break;
                }
                (ranking<k)?(left=mid+1):(right=mid-1);
            }
            if(found) break;
        }
    }
    
    return 0;
}