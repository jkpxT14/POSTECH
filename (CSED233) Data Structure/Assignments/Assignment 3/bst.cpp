#include <iostream>
#include <string>
#include "tree.h"
#include "bst.h"

using namespace std;

#define elif else if

int BinarySearchTree::insertion(int key){
    Node *new_node = new Node(key);
    if(_root==NULL){
        _root=new_node;
        return 0;
    }
    Node *cur(_root), *parent(NULL);
    while(cur!=NULL){
        parent=cur;
        if(key==cur->key){
            delete new_node;
            return -1;
        } elif(key<cur->key){
            cur=cur->left;
        } else{
            cur=cur->right;
        }
    }
    if(key<parent->key){
        parent->left=new_node;
    } else{
        parent->right=new_node;
    }
    return 0;
}

int BinarySearchTree::deletion(int key){
    Node *cur(_root), *parent(NULL);
    while(cur!=NULL && cur->key!=key){
        parent=cur;
        if(key<cur->key){
            cur=cur->left;
        } else{
            cur=cur->right;
        }
    }
    if(cur==NULL){
        return -1;
    }
    if(cur->left!=NULL && cur->right!=NULL){
        Node *succ_parent(cur), *succ(cur->right);
        while(succ->left!=NULL){
            succ_parent=succ;
            succ=succ->left;
        }
        cur->key=succ->key;
        cur=succ;
        parent=succ_parent;
    }
    Node *child;
    if(cur->left!=NULL){
        child=cur->left;
    } elif(cur->right!=NULL){
        child=cur->right;
    } else{
        child=NULL;
    }
    if(parent==NULL){
        _root=child;
    } elif(parent->left==cur){
        parent->left=child;
    } else{
        parent->right=child;
    }
    delete cur;
    return 0;
}

int BinarySearchTree::kth(int k){
    if(k<=0){
        return -1;
    }
    Node *stack[100];
    int top(-1);
    Node *cur(_root);
    int count(0);
    while(cur!=NULL || top>=0){
        while(cur!=NULL){
            stack[++top]=cur;
            cur=cur->left;
        }
        cur=stack[top--];
        ++count;
        if(count==k){
            return cur->key;
        }
        cur=cur->right;
    }
    return -1;
}