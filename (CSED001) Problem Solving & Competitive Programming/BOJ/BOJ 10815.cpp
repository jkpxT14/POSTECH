#include <iostream>

using namespace std;

void swapF(int *a, int *b){
    int temp;
    temp=*a;
    *a=*b;
    *b=temp;
}

void quickSort(int arr[], int left_0, int right_0){
    if(right_0-left_0==1){
        if(arr[left_0]>arr[right_0]){
            swapF(&arr[left_0], &arr[right_0]);
        }
        return;
    }
    int pivot=arr[(left_0+right_0)/2];
    int left=left_0;
    int right=right_0;
    do{
        while(arr[left]<pivot)
            left++;
        while(arr[right]>pivot)
            right--;
        if(left<right){
            swapF(&arr[left], &arr[right]);
            left++;
            right--;
        }
    } while(left<right);
    if(left_0<right){
        quickSort(arr, left_0, right);
    }
    if(left<right_0){
        quickSort(arr, left, right_0);
    }
}

bool binarySearch(int arr[], int left_0, int right_0, int check){
    int left=left_0, right=right_0, mid;
    while(left<=right){
        mid=(left+right)/2;
        if(arr[mid]>check){
            right=mid-1;
        }
        else if(arr[mid]==check){
            return true;
        }
        else{
            left=mid+1;
        }
    }
    return false;
}

int main(){
    ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
    int N, card[500000], M, cardCheck;
    cin>>N;
    for(int i=0; i<N; i++){
        cin>>card[i];
    }
    quickSort(card, 0, N-1);
    cin>>M;
    for(int i=0; i<M; i++){
        cin>>cardCheck;
        if(binarySearch(card, 0, N-1, cardCheck)){
            cout<<"1 ";
        }
        else{
            cout<<"0 ";
        }
    }
    return 0;
}