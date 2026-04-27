#include <stdio.h>

int main(){
    int A, B, C, brkEvenPoint;
    scanf("%d %d %d", &A, &B, &C);
    if(B==C){
        printf("-1");
        return 0;
    }
    brkEvenPoint=A/(C-B)+1;
    if(brkEvenPoint<0){
        printf("-1");
        return 0;
    }
    printf("%d", brkEvenPoint);
    return 0;
}