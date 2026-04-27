#include <stdio.h>

int main(){
    int N, cnt=0;
    scanf("%d", &N);
    while(N>0 && N%5!=0){
        N-=3;
        cnt++;
    }
    if(N<0){
        printf("-1");
        return 0;
    }
    printf("%d", N/5+cnt);
    return 0;
}