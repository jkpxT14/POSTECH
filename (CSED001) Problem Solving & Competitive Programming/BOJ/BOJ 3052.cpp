#include <stdio.h>

int main(){
    int temp, sum=0;
    bool check[42];
    for(int i=0; i<=41; i++){
        check[i]=false;
    }
    for(int i=0; i<10; i++){
        scanf("%d", &temp);
        if(!check[temp%42]){
            check[temp%42]=true;
            sum++;
        }
    }
    printf("%d", sum);
    return 0;
}