// BOJ 5430: Integer Lists

#include <bits/stdc++.h>

using namespace std;

using ll=long long;

using pqig=priority_queue<int, vector<int>, greater<int>>;
using pqllg=priority_queue<long long, vector<long long>, greater<long long>>;
using mii=map<int, int>;
using umii=unordered_map<int, int>;

#define elif else if

const auto strnpos=string::npos;

template <typename TN1, typename TN2>
pair<TN1, TN2> operator+(const pair<TN1, TN2> &x, const pair<TN1, TN2> &y){
    return {x.first+y.first, x.second+y.second};
}

template <typename TN1, typename TN2>
pair<TN1, TN2> & operator+=(pair<TN1, TN2> &x, const pair<TN1, TN2> &y){
    x.first+=y.first;
    x.second+=y.second;
    return x;
}

template <typename TN>
void print_1D(const vector<TN> &v, int i1, int i2){
    for(int i(i1); i<=i2; ++i){
        cout<<v[i]<<' ';
    }
    cout<<'\n';
}

template <typename TN1, typename TN2>
void print_1D(const vector<pair<TN1, TN2>> &v, int i1, int i2){
    for(int i(i1); i<=i2; ++i){
        cout<<'('<<v[i].first<<", "<<v[i].second<<") ";
    }
    cout<<'\n';
}

template <typename TN>
void print_2D(const vector<vector<TN>> &v, int i1, int i2, int j1, int j2){
    for(int i(i1); i<=i2; ++i){
        for(int j(j1); j<=j2; ++j){
            cout<<v[i][j]<<' ';
        }
        cout<<'\n';
    }
}

// const ll mdls(1000000007LL);
// const int INF(INT_MAX/4);
// const int offset(500000);
// const vector<pair<int, int>> delta{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}, UDLR{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int T; cin>>T;
    while(T--){
        string p; cin>>p;
        int n; cin>>n;
        deque<int> dq;
        if(!n){
            string s; cin>>s;
        } else{
            for(int i(0); i<2*n+1; ++i){
                if(i%2==0){
                    char c; cin>>c;
                } else{
                    int d; cin>>d;
                    dq.push_back(d);
                }
            }
        }
        bool reversed(false), err(false);
        for(int i(0); i<p.size(); ++i){
            if(p[i]=='R'){
                reversed=!reversed;
            } elif(p[i]=='D'){
                if(dq.empty()){
                    err=true;
                    break;
                } else{
                    if(!reversed){
                        dq.pop_front();
                    } else{
                        dq.pop_back();
                    }
                }
            }
        }
        if(err){
            cout<<"error\n";
        } else{
            cout<<'[';
            if(!reversed){
                for(int i(0); i<dq.size(); ++i){
                    cout<<dq[i];
                    if(i!=dq.size()-1){
                        cout<<',';
                    }
                }
            } else{
                for(int i(dq.size()-1); i>=0; --i){
                    cout<<dq[i];
                    if(i!=0){
                        cout<<',';
                    }
                }
            }
            cout<<"]\n";
        }
    }

    return 0;
}