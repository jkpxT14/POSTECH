#include <iostream>

using namespace std;

typedef struct Instruction {
  string command;

  int value;
  int priority;

} Instruction;

typedef struct InstructionSequence {
  Instruction *instructions;
  int length;
} InstructionSequence;

InstructionSequence *ParseInstructions(const char *arg);
void DeleteInstructionSequence(InstructionSequence *instr_seq);
void PrintInstructionSequence(InstructionSequence *instr_seq);

const char *const TASK_1_DEFAULT_ARGUMENT =
"[('insert',1), ('insert',3), ('insert',2), ('delete',3)]";
const char *const TASK_2_DEFAULT_ARGUMENT = "[('e', 10), ('e', 20), ('s', 0), ('d', 0), ('s', 0)]";
const char *const TASK_3_DEFAULT_ARGUMENT =
    "[('e',5), ('e',3), ('d',0), ('s', 0)]";
const char *const TASK_4_DEFAULT_ARGUMENT = "[('init', 5), ('show', NULL)]";
const char *const TASK_5_DEFAULT_ARGUMENT =
    "[('a', 2), ('a', 1), ('a', 5), ('a', 6), ('a', 2), ('a', 3), ('c', 0)]";