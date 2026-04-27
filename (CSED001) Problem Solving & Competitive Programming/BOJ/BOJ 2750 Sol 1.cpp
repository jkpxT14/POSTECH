#include <stdio.h>

//Call By Address
void swap(int* a, int* b){
    int temp;
    temp=*a;
    *a=*b;
    *b=temp;
}

int main(){
    int N, arr[1000];
    scanf("%d", &N);
    for(int i=0; i<N; i++){
        scanf("%d", &arr[i]);
    }
    //Bubble Sort
    for(int i=0; i<N-1; i++){
        for(int j=0; j<N-1; j++){
            if(arr[j]>arr[j+1]){
                swap(&arr[j], &arr[j+1]);
            }
        }
    }
    for(int i=0; i<N; i++){
        printf("%d\n", arr[i]);
    }
}