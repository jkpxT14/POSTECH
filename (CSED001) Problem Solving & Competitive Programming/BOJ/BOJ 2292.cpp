#include <stdio.h>

int main(){
    int N, left=2, right=7, needToAddL=6, needToAddR=12;
    scanf("%d", &N);
    if(N==1){
        printf("1");
        return 0;
    }
    for(int i=2; ; i++){
        if(N>=left && N<=right){
            printf("%d", i);
            return 0;
        }
        left+=needToAddL;
        right+=needToAddR;
        needToAddL+=6;
        needToAddR+=6;
    }
}