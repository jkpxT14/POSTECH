#pragma once
#include <iostream>
#include <string>

using namespace std;

#define elif else if

class ClosedHashFunction{
    public:
        ClosedHashFunction(int m): index_size(m) {};

        int closedhashing(int key, int attempt);
        int closedhashing1(int key);
        int closedhashing2(int attempt);
        int *offsets;

    private:
        int key_size;
        int index_size;
};