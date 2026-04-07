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

const char *const TASK_1_DEFAULT_ARGUMENT = "C(H(O))(I) 0";
const char *const TASK_2_DEFAULT_ARGUMENT= "1(2)(3)";
const char *const TASK_3_DEFAULT_ARGUMENT = "1(2)(3)";
const char *const TASK_4_DEFAULT_ARGUMENT =
    "[('insert',5),('insert',1),('insert',9),('remove',1)]";
const char *const TASK_5_DEFAULT_ARGUMENT =
        "[('A(B(D)(E))(C(F)(G))',NULL),('D',NULL),('G',NULL)]";
