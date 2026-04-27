#include <stdio.h>

int main(){
    int H, M;
    scanf("%d %d", &H, &M);
    if(M>=45){
        M-=45;
    }
    else{
        H--, M+=15;
    }
    if(H==-1){
        H=23;
    }
    printf("%d %d", H, M);
}