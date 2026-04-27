// BOJ 14215: 세 막대

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

    vi a(3);
    for(int i=0; i<3; i++){
        cin>>a[i];
    }
    sort(a.begin(), a.end());
    if(a[0]+a[1]>a[2]) cout<<a[0]+a[1]+a[2];
    else cout<<(a[0]+a[1])*2-1;

    return 0;
}