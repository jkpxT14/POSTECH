#include <iostream>
#include <string>

using namespace std;

#define elif else if

class sortAlg{
    public:
        sortAlg();

        int arr_size;
        int *arr;

        void printArray(ofstream &);
        void bubbleSort(ofstream &);
        int partition(int left, int right);
        void quickSort(ofstream &, int left, int right);
};