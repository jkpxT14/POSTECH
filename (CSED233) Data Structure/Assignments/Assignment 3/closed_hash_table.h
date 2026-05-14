#pragma once
#include <iostream>
#include <string>
#include "closed_hash_function.h"

using namespace std;

#define elif else if

enum ClosedTableState{
    CLOSED_EMPTY,
    CLOSED_OCCUPIED,
};

class ClosedHashTable{
    public:
        ClosedHashTable(int table_size, int k, int d1, ClosedHashFunction *hf);
        ~ClosedHashTable();

        void print_table(ostream &out);
        void insert(int key);

    private:
        const int table_size;
        const int k;
        const int d1;
        ClosedHashFunction *hf;
        int *table;
        ClosedTableState *states;

        int count;
        bool failed;
};