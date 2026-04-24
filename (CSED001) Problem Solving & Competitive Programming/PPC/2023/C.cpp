// BOJ 28099: 이상한 배열

#include <bits/stdc++.h>

using namespace std;

using ll=long long;
using ull=unsigned long long;

#define elif else if

const auto strnpos(string::npos);

template <typename TN1, typename TN2>
pair<TN1, TN2> operator +(const pair<TN1, TN2> &x, const pair<TN1, TN2> &y){
    return {x.first+y.first, x.second+y.second};
}

template <typename TN1, typename TN2>
pair<TN1, TN2> &operator +=(pair<TN1, TN2> &x, const pair<TN1, TN2> &y){
    x.first+=y.first; x.second+=y.second;
    return x;
}

// const ll mdls(1000000007LL);
// const int INF(INT_MAX/4);
// const int offset(500000);
// const array<pair<int, int>, 4> delta{{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int T; cin>>T;
    while(T--){
        int N; cin>>N;
        vector<int> A(N+1);
        vector<int> first(N+1, -1), last(N+1, -1);
        for(int i(1); i<=N; ++i){
            cin>>A[i];
            if(first[A[i]]==-1){
                first[A[i]]=i;
            }
            last[A[i]]=i;
        }
        set<int> s;
        bool ok(true);
        for(int i(1); i<=N; ++i){
            if(first[A[i]]==i){
                s.insert(A[i]);
            }
            if(!s.empty() && A[i]>*s.begin()){
                ok=false;
                break;
            }
            if(last[A[i]]==i){
                auto it(s.find(A[i]));
                if(it!=s.end()){
                    s.erase(it);
                }
            }
        }
        cout<<(ok?"Yes":"No")<<'\n';
    }

    return 0;
}