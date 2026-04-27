#include <stdio.h>

int main(){
    int A, B, C, product, check[10];
    scanf("%d %d %d", &A, &B, &C);
    for(int i=0; i<=9; i++){
        check[i]=0;
    }
    product=A*B*C;
    while(product!=0){
        check[product%10]++;
        product/=10;
    }
    for(int i=0; i<=9; i++){
        printf("%d\n", check[i]);
    }
    return 0;
}