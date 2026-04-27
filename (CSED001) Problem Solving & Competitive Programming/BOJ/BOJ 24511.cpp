// BOJ 24511: queuestack

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

using namespace std;

using ll=long long;
using vi=vector<int>;
using vb=vector<bool>;
using si=stack<int>;
using qi=queue<int>;
using dqi=deque<int>;
using pii=pair<int, int>;
using vpii=vector<pair<int, int>>;
using dqpii=deque<pair<int, int>>;
using vvpii=vector<vector<pair<int, int>>>;

// #define INT_MAX 2147483647

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    // cout.tie(nullptr);

    int N, M;
    dqi qs;
    cin>>N;
    vi A(N);
    for(int i=0; i<N; i++) cin>>A[i];
    for(int i=0; i<N; i++){
        int B;
        cin>>B;
        if(A[i]==0) qs.push_back(B);
    }
    cin>>M;
    for(int i=0; i<M; i++){
        int C;
        cin>>C;
        qs.push_front(C);
        cout<<qs.back()<<" ";
        qs.pop_back();
    }

    return 0;
}