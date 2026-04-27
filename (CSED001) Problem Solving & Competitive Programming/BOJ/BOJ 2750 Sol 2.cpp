#include <stdio.h>

struct minIn_{
    int val;
    int index;
} minIn;

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
    //Selection Sort
    for(int i=0; i<N-1; i++){
        minIn.val=1001;
        minIn.index=-1;
        for(int j=i; j<N; j++){
            if(minIn.val>arr[j]){
                minIn.val=arr[j];
                minIn.index=j;
            }
        }
        swap(&arr[i], &arr[minIn.index]);
    }
    for(int i=0; i<N; i++){
        printf("%d\n", arr[i]);
    }
}