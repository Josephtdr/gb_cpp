#ifndef H_CPU
#define H_CPU

#include "consts.h"
#include "registers.h"
#include "memoryBus.h"

class CPU
{
public:
    Registers registers{};
    word_t pc{};
    MemoryBus memory{};

    void cycle();

    void execute();

private:
    using opcodeFnPtr = word_t(CPU::*)();

    opcodeFnPtr instructionTable[c_INSTRUCTION_TABLE_SIZE]{};
    opcodeFnPtr alternateInstructionTable[c_INSTRUCTION_TABLE_SIZE]{};
    

    word_t OP_0x00();


    word_t OP_OxCB(); //Prefix Instruction
};



#endif