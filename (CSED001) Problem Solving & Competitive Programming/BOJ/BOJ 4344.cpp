#include <stdio.h>

int main(){
    int C, N, cnt=0;
    double score[1000], avg=0;
    scanf("%d", &C);
    for(int i=0; i<C; i++){
        scanf("%d", &N);
        for(int j=0; j<N; j++){
            scanf("%lf", &score[j]);
            avg+=score[j];
        }
        avg/=(double)N;
        for(int j=0; j<N; j++){
            if(score[j]>avg){
                cnt++;
            }
        }
        printf("%.3lf%\n", (double)cnt/(double)N*100);
        avg=0;
        cnt=0;
    }
    return 0;
}