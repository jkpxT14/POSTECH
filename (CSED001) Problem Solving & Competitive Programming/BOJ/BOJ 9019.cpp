// BOJ 9019: DSLR

#include <bits/stdc++.h>

using namespace std;

using ll=long long;

using pqig=priority_queue<int, vector<int>, greater<int>>;
using pqllg=priority_queue<long long, vector<long long>, greater<long long>>;

#define elif else if

const auto strnpos=string::npos;

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

void BFS(int from, int to){
    deque<pair<int, string>> q;
    vector<bool> vst(10000, false);
    q.push_back({from, ""});
    vst[from]=true;
    while(!q.empty()){
        pair<int, string> cur(q.front()); q.pop_front();
        if(cur.first==to){
            cout<<cur.second<<'\n';
            return;
        }
        int D((cur.first*2)%10000);
        if(!vst[D]){
            q.push_back({D, cur.second+"D"});
            vst[D]=true;
        }
        int S(((cur.first==0)?9999:(cur.first-1)));
        if(!vst[S]){
            q.push_back({S, cur.second+"S"});
            vst[S]=true;
        }
        int L(((cur.first%1000)*10)+(cur.first/1000));
        if(!vst[L]){
            q.push_back({L, cur.second+"L"});
            vst[L]=true;
        }
        int R((cur.first/10)+((cur.first%10)*1000));
        if(!vst[R]){
            q.push_back({R, cur.second+"R"});
            vst[R]=true;
        }
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int T; cin>>T;
    while(T--){
        int A, B; cin>>A>>B;
        BFS(A, B);
    }

    return 0;
}