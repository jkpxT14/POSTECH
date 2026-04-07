// Last Update on 20260407 by Jungwoo Kim(20250205, jkpxt14@postech.ac.kr)

#pragma once

#include <iostream>
#include <string>

using namespace std;

using ll=long long;
using ull=unsigned long long;

#define elif else if

#define MAX_SIZE 100

struct pq_element{
    int priority;
    int value;
};

class PriorityQueue{
    private:
        pq_element heap[MAX_SIZE];
        int size;

        void swap(int node1, int node2);
        void heapifyUp(int node);
        void heapifyDown(int node);
        int findNodeByValue(int value);

    public:
        PriorityQueue(){
            size=0;
        }

        string printPQ();
        bool empty();
        int getSize();
        bool insert(int priority, int value);
        pq_element getMax();
        bool removeMax();
        bool replaceMax(int value);
        bool removeValue(int value);
};