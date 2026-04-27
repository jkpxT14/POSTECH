#include <stdio.h>
#include <math.h>

int main(){
    int A, B, temp, res=0, i=0;
    scanf("%d %d", &A, &B);
    while(B!=0){
        temp=B%10;
        B/=10;
        printf("%d\n", A*temp);
        res+=A*temp*pow(10, i);
        i++;
    }
    printf("%d", res);
}