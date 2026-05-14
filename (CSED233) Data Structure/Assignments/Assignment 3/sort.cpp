#include <fstream>
#include <iostream>
#include <string>
#include "sort.h"

using namespace std;

#define elif else if

sortAlg::sortAlg(){
    arr_size=0;
    arr = new int[5000];
}

void sortAlg::printArray(ofstream &fout){
    string answer;
    for(int i(0); i<=arr_size-2; ++i){
        answer+=to_string(arr[i])+" ";
    }
    answer+=to_string(arr[arr_size-1]);
    fout<<answer<<endl;
}

void sortAlg::bubbleSort(ofstream &fout){
    for(int i(0); i<=arr_size-2; ++i){
        bool swapped(false);
        for(int j(arr_size-1); j>=i+1; --j){
            if(arr[j]<arr[j-1]){
                int temp(arr[j]);
                arr[j]=arr[j-1];
                arr[j-1]=temp;
                swapped=true;
            }
        }
        if(!swapped){
            break;
        }
        printArray(fout);
    }
}

int sortAlg::partition(int left, int right){
    int pivot(arr[left]);
    int l(left+1), r(right);
    while(l<=r){
        while(l<=right && arr[l]<pivot) ++l;
        while(r>=left+1 && arr[r]>=pivot) --r;
        if(l<r){
            int temp(arr[l]);
            arr[l]=arr[r];
            arr[r]=temp;
        }
    }
    arr[left]=arr[r];
    arr[r]=pivot;
    return r;
}

void sortAlg::quickSort(ofstream &fout, int left, int right){
    printArray(fout);
    int stackLeft[5000], stackRight[5000];
    int top(-1);
    stackLeft[++top]=left;
    stackRight[top]=right;
    while(top>=0){
        int curLeft(stackLeft[top]), curRight(stackRight[top--]);
        if(curLeft>=curRight){
            continue;
        }
        int pivot(partition(curLeft, curRight));
        printArray(fout);
        if (curRight-pivot>=2){
            stackLeft[++top]=pivot+1;
            stackRight[top]=curRight;
        }
        if(pivot-curLeft>=2){
            stackLeft[++top]=curLeft;
            stackRight[top]=pivot-1;
        }
    }
}