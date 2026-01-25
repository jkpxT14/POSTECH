#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <numeric>
#include <utility>
#include <tuple>
// #include <limits>
#include <cmath>
#include <bitset>

using namespace std;

using ll=long long;
using vi=vector<int>;
using vll=vector<long long>;
using vd=vector<double>;
using vb=vector<bool>;
using sti=stack<int>;
using qi=queue<int>;
using dqi=deque<int>;
using dqll=deque<long long>;
using pii=pair<int, int>;
using vvi=vector<vector<int>>;
using vpii=vector<pair<int, int>>;
using vpipii=vector<pair<int, pair<int, int>>>;
using dqpii=deque<pair<int, int>>;
using vvpii=vector<vector<pair<int, int>>>;

#define INT_MAX 2147483647 // = 2^31-1
#define INT_MIN -2147483648
#define LLONG_MAX 9223372036854775807 // = 2^63-1
#define LLONG_MIN -9223372036854775808

// 0b(2), 0(8), 0x(16)
// 0-based index vs 1-based index
// vector = vec = v
// stack = sta = st
// queue = que = q
// deque = dq
// ans = answer
// cur = current
// coordinate = coord
// adjacency matrix = adj_mat

// const int INF=INT_MAX/4; // INF = INFinity

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    
    cout<<fixed;
    cout.precision(10);

    int s=0b01100100;
    const int M=8, N=3;

    cout<<s<<"\n";

    // turn on the Nth bit
    s|=(1<<N);
    cout<<s<<"\n";

    // turn off the Nth bit
    s&=~(1<<N);
    cout<<s<<"\n";

    // toggle the Nth bit
    s^=(1<<N);
    cout<<s<<"\n";

    // test the Nth bit
    if(s&(1<<N)) cout<<"on";
    else cout<<"off";
    cout<<"\n";

    // extract the least significant set bit(=lowest set bit=rightmost set bit)
    int idx=(int)log2((s&-s)); // int idx=(s&~s+1)
    cout<<idx<<"\n";
    
    // turn off the lowest set bit
    s&=(s-1);
    cout<<s<<"\n";

    // turn on all lower M bits
    s|=(1<<M)-1;
    cout<<s<<"\n";

    cout<<"\n";

    // enumerate all subsets of a N-element universe
    for(int i=0; i<(1<<N); i++){
        for(int j=0; j<N; j++){
            if(i==0){
                cout<<"empty set";
                break;
            }
            if(i&(1<<j)) cout<<j<<" ";
        }
        cout<<"\n";
    }

    cout<<"\n";

    // set represented as a bitmask / bit based set
    // empty set
    int empty_set(0);
    cout<<empty_set<<"\n";

    // universal set
    int universal_set((1<<8)-1);
    cout<<universal_set<<"\n";

    int A(0b11100010), B(0b00010011), C;

    // insert element
    A|=(1<<3);
    cout<<A<<"\n";

    // erase element
    A&=~(1<<1);
    cout<<A<<"\n";

    // membership test / check if element is in the set
    if(A&(1<<3)) cout<<"Y";
    else cout<<"N";
    cout<<"\n";

    // toggle element
    A^=(1<<6);
    cout<<A<<"\n";

    // union
    C=A|B;
    cout<<C<<"\n";

    // intersection
    C=A&B;
    cout<<C<<"\n";

    // difference
    C=A&(~B);
    cout<<C<<"\n";

    // symmetric difference
    C=A^B;
    cout<<C<<"\n";

    // cardinality / population count
    int cardinality=0;
    int D(A);
    while(D!=0){
        cardinality+=D%2;
        D/=2;
    }
    cout<<cardinality<<"\n";

    // index of the least significant set bit
    int first=(int)log2(A&(-A)); // -A == ~A+1
    cout<<first<<"\n";

    // erase the least significant set bit
    A&=(A-1);
    cout<<A<<"\n";

    // iterate all subsets of A
    for(int subset=A; subset; subset=((subset-1)&A)){
        cout<<subset<<" ";
    }

    cout<<"\n\n";

    bitset<8> b(0b00100000);

    b.set(3);
    b.set(6);
    b.set(7);
    cout<<b<<"\n";

    b.reset(3);
    cout<<b<<"\n";

    b.flip(5);
    cout<<b<<"\n";

    cout<<b.test(5)<<"\n";

    return 0;
}