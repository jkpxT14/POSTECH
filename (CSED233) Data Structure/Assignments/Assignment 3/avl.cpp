#include <iostream>
#include <fstream>
#include "tree.h"
#include "avl.h"

using namespace std;

#define elif else if

AVLTree::AVLTree(){
    _root=NULL;
}

AVLTree::~AVLTree(){
    destroy(_root);
    _root=NULL;
}

void AVLTree::insert(char letter){
    int count(findCount(_root, letter));
    if(count>=1){
        _root=deleteNode(_root, letter, count);
    }
    _root=insertNode(_root, letter, count+1);
}

void AVLTree::printInOrder(ofstream &fout){
    bool first(true);
    printInOrder(_root, fout, first);
    fout<<endl;
}

int AVLTree::height(Node *node){
    if(node==NULL){
        return 0;
    }
    return node->height;
}

int AVLTree::maxValue(int a, int b){
    return (a>b)?a:b;
}

void AVLTree::updateHeight(Node *node){
    if(node!=NULL){
        node->height=maxValue(height(node->left), height(node->right))+1;
    }
}

int AVLTree::getBalance(Node *node){
    if(node==NULL){
        return 0;
    }
    return height(node->left)-height(node->right);
}

int AVLTree::compareNode(char letter, int count, Node *node){
    if(count<node->count){
        return -1;
    }
    if(count>node->count){
        return 1;
    }
    if(letter<node->letter){
        return -1;
    }
    if(letter>node->letter){
        return 1;
    }
    return 0;
}

Node *AVLTree::rightRotate(Node *node){
    Node *left(node->left), *middle(left->right);
    left->right=node;
    node->left=middle;
    updateHeight(node);
    updateHeight(left);
    return left;
}

Node *AVLTree::leftRotate(Node *node){
    Node *right(node->right), *middle(right->left);
    right->left=node;
    node->right=middle;
    updateHeight(node);
    updateHeight(right);
    return right;
}

Node *AVLTree::rebalance(Node *node){
    if(node==NULL){
        return NULL;
    }
    updateHeight(node);
    int balance(getBalance(node));
    if(balance>=2){
        if(getBalance(node->left)<=-1){
            node->left=leftRotate(node->left);
        }
        return rightRotate(node);
    }
    if(balance<=-2){
        if(getBalance(node->right)>=1){
            node->right=rightRotate(node->right);
        }
        return leftRotate(node);
    }
    return node;
}

Node *AVLTree::insertNode(Node *node, char letter, int count){
    if(node==NULL){
        return new Node(letter, count);
    }
    int cmp(compareNode(letter, count, node));
    if(cmp==-1){
        node->left=insertNode(node->left, letter, count);
    } elif(cmp==1){
        node->right=insertNode(node->right, letter, count);
    } else{
        return node;
    }
    return rebalance(node);
}

Node *AVLTree::findMin(Node *node){
    while(node->left!=NULL){
        node=node->left;
    }
    return node;
}

Node *AVLTree::deleteNode(Node *node, char letter, int count){
    if(node==NULL){
        return NULL;
    }
    int cmp(compareNode(letter, count, node));
    if(cmp==-1){
        node->left=deleteNode(node->left, letter, count);
    } elif(cmp==1){
        node->right=deleteNode(node->right, letter, count);
    } else{
        if(node->left==NULL || node->right==NULL){
            Node *child;
            if(node->left!=NULL){
                child=node->left;
            } elif(node->right!=NULL){
                child=node->right;
            } else{
                child=NULL;
            }
            delete node;
            return child;
        }
        Node *successor(findMin(node->right));
        node->letter=successor->letter;
        node->count=successor->count;
        node->right=deleteNode(node->right, successor->letter, successor->count);
    }
    return rebalance(node);
}

int AVLTree::findCount(Node *node, char letter){
    if(node==NULL){
        return 0;
    }
    if(node->letter==letter){
        return node->count;
    }
    int count(findCount(node->left, letter));
    if(count!=0){
        return count;
    }
    return findCount(node->right, letter);
}

void AVLTree::printInOrder(Node *node, ofstream &fout, bool &first){
    if(node==NULL){
        return;
    }
    printInOrder(node->left, fout, first);
    if(!first){
        fout<<" ";
    }
    fout<<"("<<node->letter<<", "<<node->count<< ")";
    first=false;
    printInOrder(node->right, fout, first);
}

void AVLTree::destroy(Node *node){
    if(node==NULL){
        return;
    }
    destroy(node->left);
    destroy(node->right);
    delete node;
}