// Last Update on 20260407 by Jungwoo Kim(20250205, jkpxt14@postech.ac.kr)

#include "heap.h"
#include <iostream>
#include <string>

using namespace std;

using ll=long long;
using ull=unsigned long long;

#define elif else if

// Task 4. Priority Queue Implementation
void PriorityQueue::swap(int node1, int node2){
    pq_element temp(heap[node1]);
    heap[node1]=heap[node2];
    heap[node2]=temp;
}

void PriorityQueue::heapifyUp(int node){
    while(node>=1){
        int parent((node-1)/2);
        if(heap[parent].priority>=heap[node].priority){
            break;
        }
        swap(parent, node);
        node=parent;
    }
}

void PriorityQueue::heapifyDown(int node){
    while(true){
        int leftChild(2*node+1);
        int rightChild(2*node+2);
        int largest(node);
        if(leftChild<=size-1 && heap[leftChild].priority>heap[largest].priority){
            largest=leftChild;
        }
        if(rightChild<=size-1 && heap[rightChild].priority>heap[largest].priority){
            largest=rightChild;
        }
        if(largest==node){
            break;
        }
        swap(node, largest);
        node=largest;
    }
}

int PriorityQueue::findNodeByValue(int value){
    for(int i(0); i<=size-1; ++i){
        if(heap[i].value==value){
            return i;
        }
    }
    return -1;
}

string PriorityQueue::printPQ(){
    string answer;
    for (int i(0); i<=size-1; ++i){
        answer+=to_string(heap[i].value);
        if(i<=size-2){
            answer+=" ";
        }
    }
    return answer;
}

bool PriorityQueue::empty(){
    return size==0;
}

int PriorityQueue::getSize(){
    return size;
}

bool PriorityQueue::insert(int priority, int value){
    // if(size>=MAX_SIZE){
    //     return false;
    // }
    if(findNodeByValue(value)!=-1){
        return false;
    }
    heap[size].priority=priority;
    heap[size].value=value;
    heapifyUp(size);
    ++size;
    return true;
}

pq_element PriorityQueue::getMax(){
    return heap[0];
}

bool PriorityQueue::removeMax(){
    if(size==0){
        return false;
    }
    --size;
    if(size>=1){
        heap[0]=heap[size];
        heapifyDown(0);
    }
    return true;
}

bool PriorityQueue::replaceMax(int value){
    if(size==0){
        return false;
    }
    int existingIndex(findNodeByValue(value));
    if(existingIndex!=-1 && existingIndex!=0){
        return false;
    }
    heap[0].priority=value;
    heap[0].value=value;
    heapifyDown(0);
    return true;
}

bool PriorityQueue::removeValue(int value){
    int index(findNodeByValue(value));
    if(index==-1){
        return false;
    }
    --size;
    if(index==size){
        return true;
    }
    heap[index]=heap[size];
    if(index>=1 && heap[index].priority>heap[(index-1)/2].priority){
        heapifyUp(index);
    } else{
        heapifyDown(index);
    }
    return true;
}