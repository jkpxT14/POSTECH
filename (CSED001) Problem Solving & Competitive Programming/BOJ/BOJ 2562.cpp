#include <stdio.h>

int main(){
    int temp, max=-2147483648, maxIndex=-1;
    for(int i=1; i<=9; i++){
        scanf("%d", &temp);
        if(temp>max){
            max=temp;
            maxIndex=i;
        }
    }
    printf("%d\n%d", max, maxIndex);
}