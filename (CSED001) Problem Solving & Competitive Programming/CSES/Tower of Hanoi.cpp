#include <bits/stdc++.h>

using namespace std;

using ll=long long;
using ull=unsigned long long;

#define elif else if

const auto strnpos(string::npos);

template <typename TN1, typename TN2>
pair<TN1, TN2> operator+(const pair<TN1, TN2> &x, const pair<TN1, TN2> &y){
    return {x.first+y.first, x.second+y.second};
}

template <typename TN1, typename TN2>
pair<TN1, TN2> & operator+=(pair<TN1, TN2> &x, const pair<TN1, TN2> &y){
    x.first+=y.first; x.second+=y.second;
    return x;
}

// const ll mdls(1000000007LL);
// const int INF(INT_MAX/4);
// const int offset(500000);
// const array<pair<int, int>, 4> delta{{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};

pair<int, vector<string>> ans;

void Tower_of_Hanoi(int N, int source, int auxiliary, int destination){
    if(N==1){
        ++ans.first;
        ans.second.push_back(format("{} {}", source, destination));
        return;
    } else{
        Tower_of_Hanoi(N-1, source, destination, auxiliary);
        Tower_of_Hanoi(1, source, auxiliary, destination);
        Tower_of_Hanoi(N-1, auxiliary, source, destination);
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int n; cin>>n;
    ans.first=0;
    Tower_of_Hanoi(n, 1, 2, 3);
    cout<<ans.first<<'\n';
    for(int i(0); i<=ans.first-1; ++i){
        cout<<ans.second[i]<<'\n';
    }

    return 0;
}