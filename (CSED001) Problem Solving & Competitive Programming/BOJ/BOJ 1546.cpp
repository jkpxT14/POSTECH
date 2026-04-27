#include <stdio.h>

int main(){
    int N, M=-2147483648;
    double testScore[1000], avg=0;
    scanf("%d", &N);
    for(int i=0; i<N; i++){
        scanf("%lf", &testScore[i]);
        if(M<(int)testScore[i]){
            M=(int)testScore[i];
        }
    }
    for(int i=0; i<N; i++){
        testScore[i]=testScore[i]/(double)M*100.0;
        avg+=testScore[i]/(double)N;
    }
    printf("%.10lf", avg);
    return 0;
}