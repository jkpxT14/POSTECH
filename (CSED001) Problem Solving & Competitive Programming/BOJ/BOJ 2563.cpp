// BOJ 2563

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <tuple>
// #include <limits>

using namespace std;

using ll=long long;
using vi=vector<int>;
using vb=vector<bool>;
using pii=pair<int, int>;
using vpii=vector<pair<int, int>>;
using vvpii=vector<vector<pair<int, int>>>;
// #define INT_MAX 2147483647

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    // cout.tie(nullptr);

    int N, black=0;
    bool paper[100][100];
    for(int i=0; i<100; i++){
        for(int j=0; j<100; j++){
            paper[i][j]=false;
        }
    }
    cin>>N;
    for(int i=0; i<N; i++){
        int l, m;
        cin>>l>>m;
        for(int j=l; j<l+10; j++){
            for(int k=m; k<m+10; k++){
                paper[j][k]=true;
            }
        }
    }
    for(int i=0; i<100; i++){
        for(int j=0; j<100; j++){
            if(paper[i][j]) black++;
        }
    }
    cout<<black;
    return 0;
}