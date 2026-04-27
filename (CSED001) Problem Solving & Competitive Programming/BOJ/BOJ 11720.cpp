#include <stdio.h>

int main(){
    int N, sum=0;
    char A;
    scanf("%d", &N);
    getchar();
    for(int i=0; i<N; i++){
        scanf("%c", &A);
        sum+=A-48;
    }
    printf("%d", sum);
    return 0;
}