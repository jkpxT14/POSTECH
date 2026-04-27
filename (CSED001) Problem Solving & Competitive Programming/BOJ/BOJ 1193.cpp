#include <stdio.h>

int main(){
    int X, row=-1, column=-1;
    scanf("%d", &X);
    if(X==1){
        //row=1, column=1;
        printf("1/1");
        return 0;
    }
    for(int i=1; ; i++){
        if(X>i*(i+1)/2 && X<=(i+1)*(i+2)/2){
            row=i+1;
            break;
        }
    }
    column=X-((row-1)*row/2);
    if(row%2==1){
        printf("%d/%d", (row+1-column), column);
    }
    else{
        printf("%d/%d", column, (row+1-column));
    }
    return 0;
}