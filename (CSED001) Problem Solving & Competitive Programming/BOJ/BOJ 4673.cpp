#include <stdio.h>

int d(int n){
    int res=0;
    res+=n;
    while(n!=0){
        res+=n%10;
        n/=10;
    }
    return res;
}

int main(){
    int temp;
    bool check[10001];
    for(int i=1; i<=10000; i++){
        check[i]=false;
    }
    for(int i=1; i<=10000; i++){
        if(check[i]){
            continue;
        }
        temp=d(i);
        while(temp<=10000){
            if(check[temp]){
                break;
            }
            check[temp]=true;
            temp=d(temp);
        }
    }
    for(int i=1; i<=10000; i++){
        if(!check[i]){
            printf("%d\n", i);
        }
    }
}