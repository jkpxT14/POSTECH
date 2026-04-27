#include <stdio.h>

int main(){
    int T, H, W, N, X, Y;
    scanf("%d", &T);
    for(int i=0; i<T; i++){
        scanf("%d %d %d", &H, &W, &N);
        if(N%H==0){
            X=N/H;
            Y=H;
        }
        else{
            X=N/H+1;
            Y=N%H;
        }
        printf("%d\n", Y*100+X);
    }
    return 0;
}