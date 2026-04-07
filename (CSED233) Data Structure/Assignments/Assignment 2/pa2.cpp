// Last Update on 20260407 by Jungwoo Kim(20250205, jkpxt14@postech.ac.kr)

#include "heap.h"
#include "tree.h"
#include "utils.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

using ll=long long;
using ull=unsigned long long;

#define elif else if

// Task 1. Traversal in a Binary Tree
void task_1(ofstream &fout, const char *tree_string){
    fout<<"[Task 1]"<<endl;
    try{
        string input(tree_string);
        size_t pos(input.rfind(' '));

        if(pos==string::npos){
            throw "Invalid input format";
        }

        string actual_tree(input.substr(0, pos));
        int traversal_type(stoi(input.substr(pos+1)));

        BinaryTree tree;
        tree.buildFromString(actual_tree.c_str());
        fout<<tree.printTraversal(traversal_type)<<endl;
    } catch(const char *e){
        cerr<<e<<endl;
    }
}

// Task 2. Zigzag Level Order of Binary Tree
void task_2(ofstream &fout, const char *tree_string){
    fout<<"[Task 2]"<<endl;
    try{
        BinaryTree tree;
        tree.buildFromString(tree_string);
        fout<<tree.zigzagLevelOrder()<<endl;
    } catch(const char *e){
        cerr<<e<<endl;
    }
}

// Task 3. Leaf, Width, Diameter of Binary Tree
int orderIndex;
int inorder[100];
int order[100];
int inorderIdx[100];

void task_3(ofstream &fout, const char *tree_string){
    fout<<"[Task 3]"<<endl;
    try{
        BinaryTree tree;
        tree.buildFromString(tree_string);
        fout<<tree.leafCount()<<" "<<tree.getWidth()<<" "<<tree.getDiameter()<<endl;
    } catch(const char *e){
        cerr<<e<<endl;
    }
}

// Task 4. Priority Queue Implementation
void task_4(ofstream &fout, InstructionSequence &instr_seq){
    fout<<"[Task 4]"<<endl;
    try{
        PriorityQueue pq;
        for(int i(0); i<=instr_seq.getLength()-1; ++i){
            string command(instr_seq.getInstruction(i).getCommand());
            if(command.compare("isEmpty")==0){
                if(pq.empty()){
                    fout<<"TRUE"<<endl;
                } else{
                    fout<<"FALSE"<<endl;
                }
            } elif(command.compare("insert")==0){
                int input_value(instr_seq.getInstruction(i).getValue());
                if(pq.getSize()>=MAX_SIZE){
                    fout<<"ERROR"<<endl;
                    exit(1);
                }
                if(!pq.insert(input_value, input_value)){
                    fout<<"ERROR"<<endl;
                }
            } elif(command.compare("getMax")==0){
                if(pq.empty()){
                    fout<<"EMPTY"<<endl;
                } else{
                    fout<<pq.getMax().value<<endl;
                }
            } elif(command.compare("delete")==0){
                if(!pq.removeMax()){
                    fout<<"EMPTY"<<endl; // "Empty"?
                }
                // if(pq.empty()){
                //     fout<<"EMPTY"<<endl; // "Empty"?
                // } else{
                //     pq.removeMax();
                // }
            } elif(command.compare("replaceMax")==0){
                int replace_value(instr_seq.getInstruction(i).getValue());
                if(!pq.replaceMax(replace_value)){
                    fout<<"ERROR"<<endl;
                }
            } elif(command.compare("remove")==0){
                int remove_value(instr_seq.getInstruction(i).getValue());
                if(!pq.removeValue(remove_value)){
                    fout<<"ERROR"<<endl;
                }
            } else{
                cerr<<"Invalid command"<<endl;
                exit(-1);
            }
        }
        fout<<pq.printPQ()<<endl;
    } catch(const char *e){
        cerr<<e<<endl;
    }
}

// Task 5. Lowest Common Ancestor
void task_5(ofstream &fout, InstructionSequence &instr_seq){
    fout<<"[Task 5]"<<endl;
    try{
        if(instr_seq.getLength()!=3){
            fout<<"NULL"<<endl;
            return;
        }

        string tree_str(instr_seq.getInstruction(0).getCommand());
        string node1_str(instr_seq.getInstruction(1).getCommand());
        string node2_str(instr_seq.getInstruction(2).getCommand());

        if(node1_str.length()!=1 || node2_str.length()!=1){
            fout<<"NULL"<<endl;
            return;
        }

        BinaryTree tree;
        tree.buildFromString(tree_str.c_str());
        char node1(node1_str[0]), node2(node2_str[0]);
        fout<<tree.getLCA(node1, node2)<<endl;
    } catch(const char *e){
        cerr<<e<<endl;
    }
}

int main(int argc, char **argv){
    string filename("submit.txt");
    int task_num(0);
    const char *instr_seq;
    int node_value;
    string result;
    InstructionSequence instr_seq_heap;

    // Open file
    ofstream fout;
    fout.open(filename, fstream::app);
    if(!fout.is_open()){
        cerr<<"Unable to open file: "<<filename<<endl;
        return -1;
    }

    // Choosing task number. Default is running ALL tasks (0)
    if(argc>=2){
        task_num=atoi(argv[1]);
    }
    if(argc>=3){
        instr_seq=argv[2];
    }

    // Running the task(s)
    switch(task_num){
        case 1:
            task_1(fout, instr_seq);
            break;
        case 2:
            task_2(fout, instr_seq);
            break;
        case 3:
            task_3(fout, instr_seq);
            break;
        case 4:
            instr_seq_heap.parseInstructions(instr_seq);
            task_4(fout, instr_seq_heap);
            break;
        case 5:
            instr_seq_heap.parseInstructions(instr_seq);
            task_5(fout, instr_seq_heap);
            break;
        case 0:
            task_1(fout, TASK_1_DEFAULT_ARGUMENT);
            task_2(fout, TASK_2_DEFAULT_ARGUMENT);
            task_3(fout, TASK_3_DEFAULT_ARGUMENT);
            instr_seq_heap.parseInstructions(TASK_4_DEFAULT_ARGUMENT);
            task_4(fout, instr_seq_heap);
            instr_seq_heap.parseInstructions(TASK_5_DEFAULT_ARGUMENT);
            task_5(fout, instr_seq_heap);
            break;
        default:
            cout<<"Wrong task number"<<endl;
    }

    fout.close();
    return 0;
}