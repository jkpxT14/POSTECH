#include "graph.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

using ll=long long;

#define elif else if

// Task 1. Undirected Graph - Count Simple Paths
void task_1(ofstream &fout, InstructionSequence &instr_seq){
    fout<<"[Task 1]"<<endl;
    try{
        Graph graph;
        for (int i(0); i<=instr_seq.getLength()-1; ++i){
            string firstNode(instr_seq.getInstruction(i).getCommand()), secondNode(instr_seq.getInstruction(i).getValueStr());
            graph.addUndirectedEdge(firstNode, secondNode);
        }
        fout<<graph.countSimplePaths()<<endl;
    } catch(const char *e){
        cerr<<e<<endl;
    }
}

// Task 2. Directed Graph - Semester Scheduling
void task_2(ofstream &fout, InstructionSequence &instr_seq){
    fout<<"[Task 2]"<<endl;
    try{
        Graph graph;
        for(int i(0); i<=instr_seq.getLength()-1; ++i){
            string firstNode(instr_seq.getInstruction(i).getCommand()), secondNode(instr_seq.getInstruction(i).getValueStr());
            graph.addDirectedEdge(firstNode, secondNode);
        }
        fout<<graph.getTopologicalSort()<<endl;
    } catch(const char *e){
        cerr<<e<<endl;
    }
}

// Task 3. Directed Graph - Strongly & Weakly Connected Components
void task_3(ofstream &fout, InstructionSequence &instr_seq){
    fout<<"[Task 3]"<<endl;
    try{
        Graph graph;
        Graph graph2;
        for(int i(0); i<=instr_seq.getLength()-1; ++i){
            string firstNode(instr_seq.getInstruction(i).getCommand()), secondNode(instr_seq.getInstruction(i).getValueStr());
            graph.addDirectedEdge(firstNode, secondNode);
            graph2.addUndirectedEdge(firstNode, secondNode);
        }
        fout<<"SCC"<<endl;
        fout<<graph.getStronglyConnectedComponents()<<endl;
        fout<<"WCC"<<endl;
        fout<<graph2.getWeaklyConnectedComponents()<<endl;
    } catch(const char *e){
        cerr<<e<<endl;
    }
}

// Task 4. Single Source Shortest Path - Dijkstra's Algorithm
void task_4(ofstream &fout, InstructionSequence &instr_seq){
    fout<<"[Task 4]"<<endl;
    try{
        Graph graph;
        for(int i(0); i<=instr_seq.getLength()-2; ++i){
            string command(instr_seq.getInstruction(i).getCommand());
            int value(instr_seq.getInstruction(i).getValue());
            string firstNode(command.substr(0, command.find('-'))), secondNode(command.substr(command.find('-')+1, command.length()));
            graph.addDirectedEdge(firstNode, secondNode, value);
        }
        string command(instr_seq.getInstruction(instr_seq.getLength()-1).getCommand());
        string source(command);
        int budget(instr_seq.getInstruction(instr_seq.getLength()-1).getValue());
        graph.dijkstra(source, budget, fout);
    } catch(const char *e){
        cerr<<e<<endl;
    }
}

// Task 5. Maximum-Cost Spanning Tree - Second-Best Tree
void task_5(ofstream &fout, InstructionSequence &instr_seq){
    fout<<"[Task 5]"<<endl;
    try{
        Graph graph;
        for(int i(0); i<=instr_seq.getLength()-1; ++i){
            string command(instr_seq.getInstruction(i).getCommand());
            if(command.compare("MST")==0){
                ll result(graph.findMaST(fout));
                if(result==-1){
                    fout<<"NO SECOND TREE"<<endl;
                } else{
                    fout<<result<<endl;
                }
            } else{
                string firstNode(command.substr(0, command.find('-'))), secondNode(command.substr(command.find('-')+1, command.length()));
                int value(instr_seq.getInstruction(i).getValue());
                graph.addUndirectedEdge(firstNode, secondNode, value);
            }
        }
    } catch(const char *e){
        cerr<<e<<endl;
    }
}

int main(int argc, char **argv){
    string filename("submit.txt");
    int task_num(0);
    InstructionSequence instr_seq;
    ofstream fout;
    fout.open(filename, fstream::app);
    if (!fout.is_open()){
        cerr<<"Unable to open file: "<<filename<<endl;
        return -1;
    }
    if(argc>=2){
        task_num=atoi(argv[1]);
    }
    if(argc>=3){
        try{
            instr_seq.parseInstructions(argv[2]);
        } catch(const char *e){
            cerr<<e<<endl;
            return -1;
        }
    }
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
            task_4(fout, instr_seq);
            break;
        case 5:
            task_5(fout, instr_seq);
            break;
        case 0:
            instr_seq.parseInstructions(TASK_1_DEFAULT_ARGUMENT);
            task_1(fout, instr_seq);
            instr_seq.parseInstructions(TASK_2_DEFAULT_ARGUMENT);
            task_2(fout, instr_seq);
            instr_seq.parseInstructions(TASK_3_DEFAULT_ARGUMENT);
            task_3(fout, instr_seq);
            instr_seq.parseInstructions(TASK_4_DEFAULT_ARGUMENT);
            task_4(fout, instr_seq);
            instr_seq.parseInstructions(TASK_5_DEFAULT_ARGUMENT);
            task_5(fout, instr_seq);
            break;
        default:
            cout<<"Wrong task number"<<endl;
    }
    fout.close();
    return 0;
}