#include <iostream>

using namespace std;

class Instruction {
public:
  void setCommand(string cmd);
  void setValue(int val);

  string getCommand();
  int getValue();

private:
  string command;
  int value;
};

class InstructionSequence {
public:
  InstructionSequence();
  ~InstructionSequence();

  int getLength();
  Instruction &getInstruction(int index);

  void parseInstructions(const char *arg);
  void printInstructions();

private:
  Instruction *instructions;
  int length;
};

const char *const TASK_1_DEFAULT_ARGUMENT =
    "[('insertion',42), ('insertion',20), ('insertion',17), ('insertion',13), "
    "('insertion',28), ('insertion',14), ('bubbleSort',NULL)]";
const char *const TASK_2_DEFAULT_ARGUMENT = "17 20 17 33 15 2 21 14";
const char *const TASK_3_DEFAULT_ARGUMENT =
    "[('insertion',40), ('insertion',20), ('insertion',60), ('insertion',10), "
    "('insertion',30), ('kth',3), ('deletion',20), ('kth',3), ('print', NULL)]";
const char *const TASK_4_DEFAULT_ARGUMENT = "aabbb";
const char *const TASK_5_DEFAULT_ARGUMENT =
    "[('M',4), ('k',3), ('d',2), ('insertion',32615), ('insertion',315), "
    "('insertion',6468), ('insertion',94833)]";
