#include <stdio.h>

int main(){
    int arr[1000000], N, min=2147483647, max=-2147483648, temp;
    scanf("%d", &N);
    for(int i=1; i<=N; i++){
        scanf("%d", &temp);
        if(temp<min){
            min=temp;
        }
        if(temp>max){
            max=temp;
        }
    }
    printf("%d %d", min, max);
}