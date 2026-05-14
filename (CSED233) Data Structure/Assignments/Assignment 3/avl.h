#pragma once
#include <fstream>
#include "bst.h"

using namespace std;

#define elif else if

class AVLTree{
    public:
        AVLTree();
        ~AVLTree();

        void insert(char letter);

        void printInOrder(ofstream &fout);

    private:
        Node *_root;

        int height(Node *node);
        int maxValue(int a, int b);
        void updateHeight(Node *node);
        int getBalance(Node *node);
        int compareNode(char letter, int count, Node *node);

        Node *rightRotate(Node *node);
        Node *leftRotate(Node *node);
        Node *rebalance(Node *node);

        Node *insertNode(Node *node, char letter, int count);
        Node *findMin(Node *node);
        Node *deleteNode(Node *node, char letter, int count);
        int findCount(Node *node, char letter);

        void printInOrder(Node *node, ofstream &fout, bool &first);
        void destroy(Node *node);
};