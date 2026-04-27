#include <stdio.h>

int main(){
    int N, M, card[100], ans=-1;
    scanf("%d %d", &N, &M);
    for(int i=0; i<N; i++){
        scanf("%d", &card[i]);
    }
    //brute force
    for(int i=0; i<N-2; i++){
        for(int j=i+1; j<N-1; j++){
            for(int k=j+1; k<N; k++){
                int sum=card[i]+card[j]+card[k];
                if(sum<=M && ans<sum){
                    ans=sum;
                }
            }
        }
    }
    printf("%d", ans);
}