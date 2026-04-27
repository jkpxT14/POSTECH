#include <stdio.h>

int main(){
    int testScore;
    scanf("%d", &testScore);
    if(testScore>=90){
        printf("A");
    }
    else if(testScore>=80){
        printf("B");
    }
    else if(testScore>=70){
        printf("C");
    }
    else if(testScore>=60){
        printf("D");
    }
    else{
        printf("F");
    }
}