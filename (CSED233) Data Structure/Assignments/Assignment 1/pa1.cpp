// Last Update on 20260319 by Jungwoo Kim(20250205)

#include "utils.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

using ll=long long;

#define elif else if

#define MAX_SIZE 100

// 1. List
void task_1(ofstream &fout, InstructionSequence *instr_seq){
    fout<<"[Task 1]"<<endl;

    int array[10000];
    int size(0);

    for(int i(0); i<=instr_seq->length-1; ++i){
        string command(instr_seq->instructions[i].command);
        int value(instr_seq->instructions[i].value);

        if(command.compare("insert")==0){
            int pos(0);
            while(pos<=size-1 && array[pos]>=value) ++pos;
            for(int j(size); j>=pos+1; --j) array[j]=array[j-1];
            array[pos]=value;
            ++size;
        } elif(command.compare("delete")==0){
            int pos(-1);
            for(int j(0); j<=size-1; ++j){
                if(array[j]==value){
                    pos=j;
                    break;
                }
            }
            if(pos==-1){
                fout<<"ERROR"<<endl;
                return;
            } else{
                for(int j(pos); j<=size-2; ++j) array[j]=array[j+1];
                --size;
            }
        } else{
            cerr<<"Invalid command"<<endl;
            exit(-1);
        }
    }

    if(!size){
        fout<<"EMPTY"<<endl;
    } else{
        for(int i(0); i<=size-1; ++i){
            fout<<array[i];
            if(i<=size-2){
                fout<<" ";
            }
        }
        fout<<endl;
    }
}

// 2. Stack
struct StackNode{
    int value;
    StackNode *next;
};

struct StackTop{
    int count;
    ll currentSum;
    StackNode *head;
};

void pushStack(int val, StackTop *top){
    StackNode *newNode = new StackNode;
    newNode->value=val; newNode->next=top->head;
    ++top->count; top->currentSum+=ll(val); top->head=newNode;
}

int popStack(StackTop *top){
    if(top->head==nullptr){
        return 0;
    }
    StackNode *tmp(top->head); int poppedValue(tmp->value);
    --top->count; top->currentSum-=ll(poppedValue); top->head=tmp->next;
    delete tmp;
    return poppedValue;
}

void task_2(ofstream &fout, InstructionSequence *instr_seq){
    fout<<"[Task 2]"<<endl;

    StackTop *myStack = new StackTop;
    myStack->count=0; myStack->currentSum=0LL; myStack->head=nullptr;

    for(int i(0); i<=instr_seq->length-1; ++i){
        string command(instr_seq->instructions[i].command);
        int value(instr_seq->instructions[i].value);

        if(command=="e"){
            pushStack(value, myStack);
        } elif(command=="d"){
            popStack(myStack);
        } elif(command=="s"){
            fout<<myStack->currentSum<<endl;
        } else{
            cerr<<"Invalid command"<<endl;
            exit(-1);
        }
    }

    while(myStack->head!=nullptr){
        popStack(myStack);
    }
    delete myStack;
}

// 3. Queue
struct QItem{
    int value;
    QItem *next;
};

void enqueue(QItem *&front, QItem *&rear, int &size, int value){
    QItem *newItem = new QItem;
    newItem->value=value; newItem->next=nullptr;
    if(rear==nullptr){
        front=newItem;
        rear=newItem;
    } else{
        rear->next=newItem;
        rear=newItem;
    }
    ++size;
}

int dequeue(QItem *&front, QItem *&rear, int &size){
    if(!size){
        return 0;
    }
    QItem *tmp(front); int ret(tmp->value);
    front=front->next;
    if(front==nullptr){
        rear=nullptr;
    }
    delete tmp;
    --size;
    return ret;
}

void task_3(ofstream &fout, InstructionSequence *instr_seq){
    fout<<"[Task 3]"<<endl;

    QItem *front(nullptr), *rear(nullptr);
    int size(0);

    for(int i(0); i<=instr_seq->length-1; ++i){
        string command(instr_seq->instructions[i].command);
        int value(instr_seq->instructions[i].value);

        if(command.compare("e")==0){
            enqueue(front, rear, size, value);
        } elif(command.compare("d")==0){
            dequeue(front, rear, size);
        } elif(command.compare("p")==0){
            if(!size){
                fout<<"EMPTY"<<endl;
            } else{
                fout<<front->value<<endl;
            }
        } elif(command.compare("s")==0){
            if(!size){
                fout<<"EMPTY"<<endl;
            } else{
                int currentSize(size);
                for(int j(0); j<=currentSize-1; ++j){
                    int x(dequeue(front, rear, size));
                    // fout<<x;
                    // if(j<=currentSize-2){
                    //     fout<<" ";
                    // }
                    fout<<x<<endl;
                    enqueue(front, rear, size, x);
                }
                // fout<<endl;
            }
        } elif(command.compare("f")==0){
            int currentSize(size);
            int foundIndex(-1);
            for(int j(0); j<=currentSize-1; ++j){
                int x(dequeue(front, rear, size));
                if(foundIndex==-1 && x==value){
                    foundIndex=j;
                }
                enqueue(front, rear, size, x);
            }
            if(foundIndex==-1){
                fout<<"NOTFOUND"<<endl;
            } else{
                fout<<foundIndex<<endl;
            }
        } elif(command.compare("r")==0){
            int currentSize(size);
            for(int j(0); j<=currentSize-1; ++j){
                int x(dequeue(front, rear, size));
                if(x!=value){
                    enqueue(front, rear, size, x);
                }
            }
        } else{
            cerr<<"Invalid command"<<endl;
            exit(-1);
        }
    }
}

// 4. Circular Queue
void task_4(ofstream &fout, InstructionSequence *instr_seq){
    fout<<"[Task 4]"<<endl;

    int queue_size(0), cnt(0), front(0), rear(0);
    int *arr(nullptr);

    for(int i(0); i<=instr_seq->length-1; ++i){
        string command(instr_seq->instructions[i].command);

        if(i==0 && command.compare("init")!=0){
            cerr<<"First command must be init"<<endl;
            exit(-1);
        }
        if(command.compare("init")==0){
            int new_size(instr_seq->instructions[i].value);
            if(arr!=nullptr){
                delete[] arr;
            }
            queue_size=new_size; cnt=0; front=0; rear=0;
            arr = new int[queue_size];
        } elif(command.compare("e")==0){
            int value(instr_seq->instructions[i].value);
            if(cnt<queue_size){
                arr[rear]=value;
                rear=(rear+1)%queue_size;
                ++cnt;
            }
        } elif(command.compare("d")==0){
            if(cnt>0){
                front=(front+1)%queue_size;
                --cnt;
            }
        } elif(command.compare("show")==0){
            if(!cnt){
                fout<<"EMPTY"<<endl;
            } else{
                for(int j(0); j<=cnt-1; ++j){
                    int idx((front+j)%queue_size);
                    fout<<arr[idx];
                    if(j<=cnt-2){
                        fout<<" ";
                    }
                }
                fout<<endl;
            }
        } elif(command.compare("rotate")==0){
            int k(instr_seq->instructions[i].value);
            if(cnt>0){
                if(k>0){
                    int shift(k%cnt);
                    for(int _(0); _<=shift-1; ++_){
                        int tmp(arr[front]);
                        front=(front+1)%queue_size;
                        arr[rear]=tmp;
                        rear=(rear+1)%queue_size;
                    }
                } elif(k<0){
                    int shift((-k)%cnt);
                    for(int _(0); _<=shift-1; ++_){
                        rear=(rear-1+queue_size)%queue_size;
                        int tmp(arr[rear]);
                        front=(front-1+queue_size)%queue_size;
                        arr[front]=tmp;
                    }
                }
            }
        } else{
            cerr<<"Invalid command"<<endl;
            exit(-1);
        }
    }

    if(arr!=nullptr){
        delete[] arr;
    }
}

// 5. Largest rectangle area in histogram
struct BarInfo{
    int height;
    int index;
};

struct HistoNode{
    BarInfo data;
    HistoNode *next;
};

struct HistoTop{
    int count;
    HistoNode *head;
};

int histogram[100];
int currentHistoSize(0);

void pushHisto(BarInfo info, HistoTop *top){
    HistoNode *newNode = new HistoNode;
    newNode->data=info; newNode->next=top->head;
    ++top->count; top->head=newNode;
}

BarInfo popHisto(HistoTop *top){
    if(top->head==nullptr){
        return BarInfo{0, 0};
    }
    HistoNode *tmp(top->head); BarInfo ret(tmp->data);
    --top->count; top->head=tmp->next;
    delete tmp;
    return ret;
}

BarInfo peekHisto(HistoTop *top){
    if(top->head==nullptr){
        return {-1, -1};
    }
    return top->head->data;
}

void task_5(ofstream &fout, InstructionSequence *instr_seq){
    fout<<"[Task 5]"<<endl;

    currentHistoSize=0;

    for(int i(0); i<=instr_seq->length-1; ++i){
        string command(instr_seq->instructions[i].command);
        int value(instr_seq->instructions[i].value);

        if(command=="a"){
            if(currentHistoSize<=99){
                histogram[currentHistoSize]=value;
                ++currentHistoSize;
            }
        } elif(command=="c"){
            ll maxArea(0);
            HistoTop *stack = new HistoTop;
            stack->count=0; stack->head=nullptr;
            for(int j(0); j<=currentHistoSize; ++j){
                int currentHeight(((j==currentHistoSize)?0:histogram[j]));
                int startIndex(j);
                while(stack->head!=nullptr && peekHisto(stack).height>currentHeight){
                    BarInfo topBar(popHisto(stack));
                    ll area(ll(topBar.height)*ll(j-topBar.index));
                    if(area>maxArea){
                        maxArea=area;
                    }
                    startIndex=topBar.index;
                }
                pushHisto({currentHeight, startIndex}, stack);
            }
            fout<<maxArea<<endl;

            while(stack->head!=nullptr){
                popHisto(stack);
            }
            delete stack;
        }
        else{
            cerr<<"Invalid command"<<endl;
            exit(-1);
        }
    }
}

int main(int argc, char **argv){
    string filename("submit.txt");
    int task_num(0);
    InstructionSequence *instr_seq;
    string expression;

    // Open file
    ofstream fout;
    fout.open(filename, fstream::app);
    if(!fout.is_open()){
        cerr<<"Unable to open file: "<<filename<<endl;
        return -1;
    }

    // Choosing task number. Default is running ALL tasks (0)
    if(argc>=2) task_num=atoi(argv[1]);
    if(argc>=3){
        try{
            instr_seq=ParseInstructions(argv[2]);
        } catch(const char *e){
            cerr<<e<<endl;
            return -1;
        }
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
            task_4(fout, instr_seq);
            break;
        case 5:
            task_5(fout, instr_seq);
            break;
        case 0:
            InstructionSequence *instr_seq_1;
            instr_seq_1=ParseInstructions(TASK_1_DEFAULT_ARGUMENT);
            task_1(fout, instr_seq_1);

            InstructionSequence *instr_seq_2;
            instr_seq_2=ParseInstructions(TASK_2_DEFAULT_ARGUMENT);
            task_2(fout, instr_seq_2);

            InstructionSequence *instr_seq_3;
            instr_seq_3=ParseInstructions(TASK_3_DEFAULT_ARGUMENT);
            task_3(fout, instr_seq_3);

            InstructionSequence *instr_seq_4;
            instr_seq_4=ParseInstructions(TASK_4_DEFAULT_ARGUMENT);
            task_4(fout, instr_seq_4);

            InstructionSequence *instr_seq_5;
            instr_seq_5=ParseInstructions(TASK_5_DEFAULT_ARGUMENT);
            task_5(fout, instr_seq_5);
            break;
        default:
            cout<<"Wrong task number"<<endl;
    }
    fout.close();
    return 0;
}