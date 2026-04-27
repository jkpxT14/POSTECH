#include <stdio.h>
#include <vector>

using namespace std;

bool check(int X){
    vector<int> seq;
    while(X!=0){
        seq.push_back(X%10);
        X/=10;
    }
    if(seq.size()==1 || seq.size()==2){
        return true;
    }
    for(int i=0; i<seq.size()-2; i++){
        if(seq[i+1]-seq[i]!=seq[i+2]-seq[i+1]){
            return false;
        }
    }
    return true;
}

int main(){
    int N, cnt=0;
    scanf("%d", &N);
    for(int i=1; i<=N; i++){
        if(check(i)){
            cnt++;
        }
    }
    printf("%d", cnt);
    return 0;
}