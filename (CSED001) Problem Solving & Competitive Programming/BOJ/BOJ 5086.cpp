// BOJ 5086: Factors And Multiples

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <tuple>
#include <limits>

using namespace std;

using ll=long long;
using vi=vector<int>;
using vb=vector<bool>;
using pii=pair<int, int>;
using vpii=vector<pair<int, int>>;
using vvpii=vector<vector<pair<int, int>>>;
#define INT_MAX 2147483647

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    // cout.tie(nullptr);

    while(true){
        int m, n;
        cin>>m>>n;
        if(m==0 && n==0) break;
        if(m%n==0) cout<<"multiple";
        else if(n%m==0) cout<<"factor";
        else cout<<"neither";
        cout<<"\n";
    }

    return 0;
}