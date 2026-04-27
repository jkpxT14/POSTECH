// BOJ 25206: 너의 평점은

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

    string course_title;
    double credit, grade_pts, credit_sum=0.0, grade_pts_sum=0.0;
    string grade;

    for(int i=0; i<20; i++){
        cin>>course_title>>credit>>grade;
        if(grade=="A+") grade_pts=4.5;
        else if(grade=="A0") grade_pts=4.0;
        else if(grade=="B+") grade_pts=3.5;
        else if(grade=="B0") grade_pts=3.0;
        else if(grade=="C+") grade_pts=2.5;
        else if(grade=="C0") grade_pts=2.0;
        else if(grade=="D+") grade_pts=1.5;
        else if(grade=="D0") grade_pts=1.0;
        else if(grade=="P") continue;
        else if(grade=="F") grade_pts=0.0;
        credit_sum+=credit;
        grade_pts_sum+=grade_pts*credit;
    }
    cout<<grade_pts_sum/credit_sum;

    return 0;
}