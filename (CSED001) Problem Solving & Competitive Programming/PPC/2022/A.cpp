// BOJ 25542: 약속 장소

#include <bits/stdc++.h>

using namespace std;

using ll=long long;
using ull=unsigned long long;

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

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    cout<<fixed<<setprecision(10);

    int N, L; cin>>N>>L;
    vector<string> candidate(N);
    cin>>candidate[0];
    int max_diff_cnt(-1);
    vector<int> change_idx;
    for(int i(1); i<=N-1; ++i){
        cin>>candidate[i];
        int diff_cnt(0);
        for(int j(0); j<=L-1; ++j){
            if(candidate[i][j]!=candidate[0][j]){
                ++diff_cnt;
            }
        }
        if(diff_cnt==2){
            if(max_diff_cnt<=1){
                for(int j(0); j<=L-1; ++j){
                    if(candidate[i][j]!=candidate[0][j]){
                        change_idx.push_back(j);
                    }
                }
            } elif(max_diff_cnt==2){
                assert(change_idx.size()==2);
                if(candidate[i][change_idx[0]]==candidate[0][change_idx[0]] && candidate[i][change_idx[1]]==candidate[0][change_idx[1]]){
                    cout<<"CALL FRIEND";
                    return 0;
                }
            }
        }
        max_diff_cnt=max(max_diff_cnt, diff_cnt);
    }
    if(max_diff_cnt<=1){
        cout<<candidate[0];
        return 0;
    } elif(max_diff_cnt>=3){
        cout<<"CALL FRIEND";
        return 0;
    }
    for(int i(0); i<=25; ++i){
        string final_candidate(candidate[0]);
        final_candidate[change_idx[0]]=char('A'+i);
        max_diff_cnt=-1;
        for(int j(0); j<=N-1; ++j){
            int diff_cnt(0);
            for(int k(0); k<=L-1; ++k){
                if(candidate[j][k]!=final_candidate[k]){
                    ++diff_cnt;
                }
            }
            max_diff_cnt=max(max_diff_cnt, diff_cnt);
        }
        if(max_diff_cnt<=1){
            cout<<final_candidate;
            return 0;
        }
    }
    for(int i(0); i<=25; ++i){
        string final_candidate(candidate[0]);
        final_candidate[change_idx[1]]=char('A'+i);
        max_diff_cnt=-1;
        for(int j(0); j<=N-1; ++j){
            int diff_cnt(0);
            for(int k(0); k<=L-1; ++k){
                if(candidate[j][k]!=final_candidate[k]){
                    ++diff_cnt;
                }
            }
            max_diff_cnt=max(max_diff_cnt, diff_cnt);
        }
        if(max_diff_cnt<=1){
            cout<<final_candidate;
            return 0;
        }
    }
    cout<<"CALL FRIEND";
    return 0;
}