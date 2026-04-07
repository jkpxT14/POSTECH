// Last Update on 20260407 by Jungwoo Kim(20250205, jkpxt14@postech.ac.kr)

#pragma once

#include <string>
#include <fstream>

using namespace std;

using ll=long long;
using ull=unsigned long long;

#define elif else if

struct Node{
    Node(char input_value): value(input_value), left(NULL), right(NULL) {}
    char value;
    Node *left;
    Node *right;
};

class BinaryTree{
    public:
        BinaryTree(){
            _root=NULL;
        };
        Node *getRoot(){
            return _root;
        }
        ~BinaryTree(){
            delete[] _root;
            _root=NULL;
        };
        void buildFromString(const char *data);

        // Task 1. Traversal in a Binary Tree
        string printTraversal(int traversal_type);

        // Task 2. Zigzag Level Order of Binary Tree
        string zigzagLevelOrder();

        // Task 3. Leaf, Width, Diameter of Binary Tree
        string leafCount();
        string getWidth();
        string getDiameter();

        // Task 5. Lowest Common Ancestor
        string getLCA(char a, char b);

    private:
        Node *_root;
        Node *_buildFromString(const char *data, int start, int end);

        // Task 1. Traversal in a Binary Tree
        void _preorder(string &list, Node *node);
        void _inorder(string &list, Node *node);
        void _postorder(string &list, Node *node);

        // Task 2. Zigzag Level Order of Binary Tree
        int _getHeight(Node *a);
        void _currentLevel(string &list, Node *a, int level, bool leftToRight);
        void _zigzagLevelOrder(string &list, Node *a);

        // Task 3. Leaf, Width, Diameter of Binary Tree
        int _countLeaves(Node *node);
        int countNodesAtLevel(Node *node, int level);
        int _getDiameter(Node *node, int &diameter);

        // Task 5. Lowest Common Ancestor
        bool checkExistence(Node *node, char target);
        Node *findLCA(Node *node, char a, char b);
};