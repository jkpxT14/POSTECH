#pragma once
#include "tree.h"

using namespace std;

#define elif else if

class BinarySearchTree: public BinaryTree{
    public:
        BinarySearchTree(){};
        ~BinarySearchTree(){};

        int insertion(int key);
        int deletion(int key);
        int kth(int k);
};