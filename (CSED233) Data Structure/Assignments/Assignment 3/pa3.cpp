#include <cctype>
#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>
#include "avl.h"
#include "bst.h"
#include "closed_hash_function.h"
#include "closed_hash_table.h"
#include "sort.h"
#include "tree.h"
#include "utils.h"

using namespace std;

#define elif else if

// Task 1. Bubble Sort
void task_1(ofstream &fout, InstructionSequence &instr_seq){
    fout<<"[Task 1]"<<endl;
    try{
        sortAlg sort;
        for(int i(0); i<=instr_seq.getLength()-1; ++i){
            string command(instr_seq.getInstruction(i).getCommand());
            if(command.compare("insertion")==0){
                sort.arr[sort.arr_size]=instr_seq.getInstruction(i).getValue();
                ++sort.arr_size;
            } elif(command.compare("bubbleSort")==0){
                sort.printArray(fout);
                sort.bubbleSort(fout);
            } else{
                cerr<<"Invalid command"<<endl;
                exit(-1);
            }
        }
    } catch(const char *e){
        cerr<<e<<endl;
    }
}

// Task 2. Non-recursice Quick sort
void parseIntArray(string str, int *arr, int &size){
    size=0;
    stringstream ss(str);
    string token;
    while(getline(ss, token, ' ')){
        if(token.empty()){
            continue;
        }
        arr[size++]=stoi(token);
    }
}

void task_2(ofstream &fout, string input){
    fout<<"[Task 2]"<<endl;
    int arr[5000];
    int size(0);
    parseIntArray(input, arr, size);
    try{
        sortAlg sort;
        for(int i(0); i<=size-1; ++i){
            sort.arr[sort.arr_size++]=arr[i];
        }
        sort.quickSort(fout, 0, sort.arr_size-1);
    } catch(const char *e){
        cerr<<e<<endl;
    }
}

// Task 3. BST Insertion/Deletion/k-th Smallest
void task_3(ofstream &fout, InstructionSequence &instr_seq){
    fout<<"[Task 3]"<<endl;
    try{
        BinarySearchTree tree;
        int ret;
        for (int i(0); i<=instr_seq.getLength()-1; ++i){
            string command(instr_seq.getInstruction(i).getCommand());
            int key(instr_seq.getInstruction(i).getValue());
            if(command.compare("insertion")==0){
                ret=tree.insertion(key);
                fout<<ret<<endl;
            } elif(command.compare("deletion")==0){
                ret=tree.deletion(key);
                fout<<ret<<endl;
            } elif(command.compare("kth")==0){
                ret=tree.kth(key);
                fout<<ret<<endl;
            } elif(command.compare("print")==0){
                string preorder(tree.preOrder()), inorder(tree.inOrder());
                if(!preorder.empty()){
                    fout<<preorder<<endl<<inorder<<endl;
                }
            } else{
                cerr<<"Invalid command"<<endl;
                exit(-1);
            }
        }
    } catch(const char *e){
        cerr<<e<<endl;
    }
}

// Task 4. Duplicate Letter Counting Problem
void task_4(ofstream &fout, string expression){
    fout<<"[Task 4]"<<endl;
    try{
        AVLTree tree;
        for(char letter: expression){
            tree.insert(tolower(letter));
        }
        tree.printInOrder(fout);
    } catch(const char *e){
        cerr<<e<<endl;
    }
}

// Task 5. Closed hash table
void task_5(ofstream &fout, InstructionSequence &instr_seq){
    fout<<"[Task 5]"<<endl;
    try{
        int M, k, d1;
        ClosedHashFunction *hf;
        ClosedHashTable *ht;
        if(instr_seq.getLength()<=1 || instr_seq.getInstruction(0).getCommand().compare("M")!=0){
            cerr<<"Invalid starting commands"<<endl;
            exit(-1);
        }
        M=instr_seq.getInstruction(0).getValue();
        k=instr_seq.getInstruction(1).getValue();
        d1=instr_seq.getInstruction(2).getValue();
        hf = new ClosedHashFunction(M);
        ht = new ClosedHashTable(M, k, d1, hf);
        for (int i(3); i<=instr_seq.getLength()-1; ++i){
            string command(instr_seq.getInstruction(i).getCommand());
            int key(instr_seq.getInstruction(i).getValue());
            if(command.compare("insertion")==0){
                ht->insert(key);
            } else{
                cout<<command<<endl;
                cerr<<"Invalid command"<<endl;
                exit(-1);
            }
        }
        ht->print_table(fout);
        delete ht;
        delete hf;
    } catch(const char *e){
        cerr<<e<<endl;
    }
}

int main(int argc, char **argv){
    string filename("submit.txt");
    int task_num(0);
    InstructionSequence instr_seq;
    string expression;
    ofstream fout;
    fout.open(filename, fstream::app);
    if(!fout.is_open()){
        cerr<<"Unable to open file: "<<filename<<endl;
        return -1;
    }
    if(argc>=2){
        task_num=atoi(argv[1]);
    }
    if(argc>=3){
        if(task_num==2){
            expression=argv[2];
        } elif(task_num==4){
            expression=argv[2];
        } else{
            try{
                instr_seq.parseInstructions(argv[2]);
            } catch(const char *e){
                cerr<<e<<endl;
                return -1;
            }
        }
    }
    switch(task_num){
        case 1:
            task_1(fout, instr_seq);
            break;
        case 2:
            task_2(fout, expression);
            break;
        case 3:
            task_3(fout, instr_seq);
            break;
        case 4:
            task_4(fout, expression);
            break;
        case 5:
            task_5(fout, instr_seq);
            break;
        case 0:
            instr_seq.parseInstructions(TASK_1_DEFAULT_ARGUMENT);
            task_1(fout, instr_seq);
            task_2(fout, TASK_2_DEFAULT_ARGUMENT);
            instr_seq.parseInstructions(TASK_3_DEFAULT_ARGUMENT);
            task_3(fout, instr_seq);
            expression = TASK_4_DEFAULT_ARGUMENT;
            task_4(fout, expression);
            instr_seq.parseInstructions(TASK_5_DEFAULT_ARGUMENT);
            task_5(fout, instr_seq);
            break;
        default:
            cout<<"Wrong task number"<<endl;
    }
    fout.close();
    return 0;
}