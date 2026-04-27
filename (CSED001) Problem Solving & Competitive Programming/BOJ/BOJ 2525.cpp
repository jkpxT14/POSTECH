#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

using ll=long long;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int A, B, C;
    cin>>A>>B>>C;
    B+=C;
    A+=B/60;
    A%=24;
    B%=60;
    cout<<A<<" "<<B;
}