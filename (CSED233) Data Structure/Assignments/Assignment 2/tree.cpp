// Last Update on 20260407 by Jungwoo Kim(20250205, jkpxt14@postech.ac.kr)

#include "tree.h"
#include <cmath>
#include <cstring>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

using ll=long long;
using ull=unsigned long long;

#define elif else if

// These are given functions. I didn't modify these functions.
int findIndex(const char *str, int start, int end){
    if(start>end){
        return -1;
    }
    string s;
    for (int i(start); i<=end; ++i){
        if (str[i]=='('){
            s.push_back(str[i]);
        } elif(str[i]==')'){
            if(s.back()=='('){
                s.pop_back();
                if (!s.length()){
                    return i;
                }
            }
        }
    }
    return -1;
}

Node *BinaryTree::_buildFromString(const char *data, int start, int end){
    if(start>end){
        return NULL;
    }
    Node *root = new Node(data[start]);
    int index(-1);
    if (start+1<=end && data[start+1]=='('){
        index=findIndex(data, start+1, end);
    }
    if(index!=-1){
        root->left=_buildFromString(data, start+2, index-1);
        root->right=_buildFromString(data, index+2, end-1);
    }
    return root;
}

void BinaryTree::buildFromString(const char *data){
    Node *root=_buildFromString(data, 0, strlen(data)-1);
    _root=root;
}

// Task 1. Traversal in a Binary Tree
void BinaryTree::_preorder(string &list, Node *node){
    if(node==NULL){
        return;
    }
    list+=node->value;
    _preorder(list, node->left);
    _preorder(list, node->right);
}

void BinaryTree::_inorder(string &list, Node *node){
    if(node==NULL){
        return;
    }
    _inorder(list, node->left);
    list+=node->value;
    _inorder(list, node->right);
}

void BinaryTree::_postorder(string &list, Node *node){
    if(node==NULL){
        return;
    }
    _postorder(list, node->left);
    _postorder(list, node->right);
    list+=node->value;
}

string BinaryTree::printTraversal(int traversal_type){
    string list("");
    if(traversal_type==0){
        _preorder(list, _root);
    } elif(traversal_type==1){
        _inorder(list, _root);
    } elif(traversal_type==2){
        _postorder(list, _root);
    }
    return list;
}

// Task 2. Zigzag Level Order of Binary Tree
int BinaryTree::_getHeight(Node *a){
    if(a==NULL){
        return -1;
    }
    int leftHeight(_getHeight(a->left));
    int rightHeight(_getHeight(a->right));
    return 1+max(leftHeight, rightHeight);
}

void BinaryTree::_currentLevel(string &list, Node *a, int level, bool leftToRight){
    if(a==NULL){
        return;
    }
    if(level==0){
        if(!list.empty()){
            list+=" ";
        }
        list+=a->value;
        return;
    }
    if(leftToRight){
        _currentLevel(list, a->left, level-1, leftToRight);
        _currentLevel(list, a->right, level-1, leftToRight);
    } elif(!leftToRight){
        _currentLevel(list, a->right, level-1, leftToRight);
        _currentLevel(list, a->left, level-1, leftToRight);
    }
}

void BinaryTree::_zigzagLevelOrder(string &list, Node *a){
    int height(_getHeight(a));
    bool leftToRight(true);
    for(int level(0); level<=height; ++level){
        _currentLevel(list, a, level, leftToRight);
        leftToRight=!leftToRight;
    }
}

string BinaryTree::zigzagLevelOrder(){
    string list("");
    _zigzagLevelOrder(list, _root);
    return list;
}

// Task 3. Leaf, Width, Diameter of Binary Tree
int BinaryTree::_countLeaves(Node *node){
    if(node==NULL){
        return 0;
    }
    if(node->left==NULL && node->right==NULL){
        return 1;
    }
    return _countLeaves(node->left)+_countLeaves(node->right);
}

string BinaryTree::leafCount(){
    return to_string(_countLeaves(_root));
}

int BinaryTree::countNodesAtLevel(Node *node, int level){
    if(node==NULL){
        return 0;
    }
    if(level==0){
        return 1;
    }
    return countNodesAtLevel(node->left, level-1)+countNodesAtLevel(node->right, level-1);
}

string BinaryTree::getWidth(){
    if(_root==NULL){
        return "0";
    }
    int height(_getHeight(_root));
    int width(0);
    for(int level(0); level<=height; ++level){
        width=max(width, countNodesAtLevel(_root, level));
    }
    return to_string(width);
}

int BinaryTree::_getDiameter(Node *node, int &diameter){
    if(node==NULL){
        return -1;
    }
    int leftHeight(_getDiameter(node->left, diameter));
    int rightHeight(_getDiameter(node->right, diameter));
    diameter=max(diameter, leftHeight+rightHeight+2);
    return 1+max(leftHeight, rightHeight);
}

string BinaryTree::getDiameter(){
    if(_root==NULL){
        return "0";
    }
    int diameter(0);
    _getDiameter(_root, diameter);
    return to_string(diameter);
}

// Task 5. Lowest Common Ancestor
bool BinaryTree::checkExistence(Node *node, char target){
    if(node==NULL){
        return false;
    }
    if(node->value==target){
        return true;
    }
    return checkExistence(node->left, target) || checkExistence(node->right, target);
}

Node *BinaryTree::findLCA(Node *node, char a, char b){
    if(node==NULL || node->value==a || node->value==b){
        return node;
    }
    Node *leftResult(findLCA(node->left, a, b));
    Node *rightResult(findLCA(node->right, a, b));
    if(leftResult!=NULL && rightResult!=NULL){
        return node;
    }
    if(leftResult!=NULL){
        return leftResult;
    }
    return rightResult;
}

string BinaryTree::getLCA(char a, char b){
    if(!checkExistence(_root, a) || !checkExistence(_root, b)){
        return "NULL";
    }
    Node *LCA(findLCA(_root, a, b));
    if(LCA==NULL){
        return "NULL";
    }
    return string(1, LCA->value);
}