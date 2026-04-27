#include <stdio.h>

int fac(int pN){
    if(pN==0 || pN==1) return 1;
    return fac(pN-1)*pN;
}

int main(){
    int N;
    scanf("%d", &N);
    printf("%d", fac(N));
}