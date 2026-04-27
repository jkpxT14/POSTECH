#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

using ll=long long;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int a, b, c;
    cin>>a>>b>>c;
    if(a==b && a==c){
        cout<<10000+1000*a;
    }
    else if(a==b || a==c){
        cout<<1000+100*a;
    }
    else if(b==c){
        cout<<1000+100*b;
    }
    else{
        cout<<max(max(a, b), c)*100;
    }
}