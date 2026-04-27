#include <stdio.h>

int main(){
    int firN, curN, tempN, digitSum=0, cycCnt=0;
    scanf("%d", &firN);
    curN=firN, tempN=firN;
    while(curN!=firN || cycCnt==0){
        while(tempN!=0){
            digitSum+=tempN%10;
            tempN/=10;
        }
        curN=10*(curN%10)+(digitSum%10);
        cycCnt++;
        digitSum=0;
        tempN=curN;
    }
    printf("%d", cycCnt);
    return 0;
}