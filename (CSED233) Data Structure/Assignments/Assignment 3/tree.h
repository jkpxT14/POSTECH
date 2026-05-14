#pragma once
#include <string>

using namespace std;

#define elif else if

class Node{
    public:
        Node(int key): key(key), height(0), left(NULL), right(NULL) {}
        Node(char letter, int count): count(count), letter(letter), height(1), left(NULL), right(NULL) {}
        int key;
        int height;
        Node *left;
        Node *right;

        char letter;
        int count;

        string print_key(){return to_string(key);};
        string print_count(){return string(1, letter)+" ("+to_string(count)+")";};
};

class BinaryTree{
    public:
        BinaryTree(){_root=NULL;};
        ~BinaryTree(){
            delete[] _root;
            _root=NULL;
        };
        string preOrder();
        string inOrder();

    protected:
        Node *_root;

    private:
        void _inOrder(string &output, Node *cur);
        void _preOrder(string &output, Node *cur);
};