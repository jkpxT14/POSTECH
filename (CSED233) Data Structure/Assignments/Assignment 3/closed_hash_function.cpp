#include <iostream>
#include <fstream>
#include <string>
#include "closed_hash_function.h"

using namespace std;

#define elif else if

int ClosedHashFunction::closedhashing(int key, int attempt){
    return (closedhashing1(key)+closedhashing2(attempt))%index_size;
}

int ClosedHashFunction::closedhashing1(int key){
    int sum(0);
    while(key>=1){
        sum+=key%10;
        key/=10;
    }
    return sum%index_size;
}

int ClosedHashFunction::closedhashing2(int attempt){
    if(attempt<=0){
        return 0;
    }
    return offsets[attempt-1];
}