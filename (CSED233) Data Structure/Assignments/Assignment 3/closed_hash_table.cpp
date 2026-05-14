#include <iostream>
#include <fstream>
#include <string>
#include "closed_hash_function.h"
#include "closed_hash_table.h"

using namespace std;

#define elif else if

ClosedHashTable::ClosedHashTable(int table_size, int k, int d1, ClosedHashFunction *hf): table_size(table_size), k(k), d1(d1) {
    this->hf=hf;
    this->table = new int[this->table_size];
    this->states = new ClosedTableState[this->table_size];
    for(int i(0); i<=this->table_size-1; ++i){
        this->states[i]=CLOSED_EMPTY;
    }
    this->count=0;
    this->failed=false;
    this->hf->offsets = new int[this->table_size];
    int d(this->d1);
    for (int i(0); i<=this->table_size-1; ++i){
        this->hf->offsets[i]=d;
        int t(2*d);
        d=(t<this->table_size?t:(((t-this->table_size)^this->k)%this->table_size));
    }
}

ClosedHashTable::~ClosedHashTable(){
    delete[] this->hf->offsets;
    delete[] this->table;
    delete[] this->states;
}

void ClosedHashTable::print_table(ostream &out){
    for(int i(0); i<=this->table_size-1; ++i){
        out<<i<<": ";
        if(this->states[i]==CLOSED_EMPTY){
            out<<"EMPTY"<<endl;
        } elif(this->states[i]==CLOSED_OCCUPIED){
            out<<this->table[i]<<endl;
        } else{
            out<<"UNKNOWN"<<endl;
        }
    }
    if(this->failed){
        out<<"FAIL"<<endl;
    }
}

void ClosedHashTable::insert(int key){
    if(this->failed){
        return;
    }
    if(this->count==this->table_size){
        this->failed=true;
        return;
    }
    int index(this->hf->closedhashing1(key));
    if(this->states[index]==CLOSED_EMPTY){
        this->table[index]=key;
        this->states[index]=CLOSED_OCCUPIED;
        ++this->count;
        return;
    }
    bool *visited = new bool[this->table_size];
    for(int i(0); i<=this->table_size-1; ++i){
        visited[i]=false;
    }
    visited[index]=true;
    for (int attempt(1); attempt<=this->table_size-1; ++attempt){
        index=this->hf->closedhashing(key, attempt);
        if(visited[index]){
            this->failed=true;
            delete[] visited;
            return;
        }
        visited[index]=true;
        if(this->states[index]==CLOSED_EMPTY){
            this->table[index]=key;
            this->states[index]=CLOSED_OCCUPIED;
            ++this->count;
            delete[] visited;
            return;
        }
    }
    this->failed=true;
    delete[] visited;
}