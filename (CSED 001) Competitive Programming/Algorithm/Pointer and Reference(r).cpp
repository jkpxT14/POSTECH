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

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    // cout.tie(nullptr);

    int M=1, N=2;
    int *p=&M;
    cout<<&M<<" "<<M<<"\n";
    cout<<&p<<" "<<p<<" "<<*p<<"\n";

    cout<<"\n";

    int &q=N;
    cout<<&N<<" "<<N<<"\n";
    cout<<&q<<" "<<q;
}