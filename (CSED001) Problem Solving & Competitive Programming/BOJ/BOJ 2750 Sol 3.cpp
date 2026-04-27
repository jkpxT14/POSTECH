#include <stdio.h>
#include <algorithm>

using namespace std;

int main(){
    int N, arr[1000], key;
    bool flag=false;
    scanf("%d", &N);
    for(int i=0; i<N; i++){
        scanf("%d", &arr[i]);
    }
    //Insertion Sort
    for(int i=1; i<N; i++){
            key=arr[i];
        for(int j=i-1; j>=0; j--){
            if(key<arr[j]){
                arr[j+1]=arr[j];
            }
            else{
                arr[j+1]=key;
                flag=true;
                break;
            }
        }
        if(!flag){
            arr[0]=key;
        }
        else{
            flag=false;
        }
//        for(int i=0; i<N; i++){
//            printf("%d ", arr[i]);
//        }
//        printf("\n");
    }
    for(int i=0; i<N; i++){
        printf("%d\n", arr[i]);
    }
}