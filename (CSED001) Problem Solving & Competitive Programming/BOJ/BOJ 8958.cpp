#include <stdio.h>
#include <string.h>

int main(){
    int T, cnt=0, sum=0;
    char quizRes[80];
    scanf("%d", &T);
    for(int i=0; i<T; i++){
        scanf("%s", quizRes);
        for(int i=0; i<(int)strlen(quizRes); i++){
            if(quizRes[i]=='O'){
                cnt++;
                sum+=cnt;
            }
            else{
                cnt=0;
            }
        }
        printf("%d\n", sum);
        sum=0;
        cnt=0;
    }
}