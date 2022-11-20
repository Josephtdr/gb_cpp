#ifndef H_CPU
#define H_CPU

#include "consts.h"
#include "registers.h"
#include "memoryBus.h"

class CPU
{
private:
    MemoryBus memory{};
    Registers registers{};
    word_t pc{};
    word_t sp{};
    
public:
    CPU();

    void cycle();


private:
    word_t execute(byte_t instructionByte, bool prefixed);

    using opcodeFnPtr = word_t(CPU::*)();
    opcodeFnPtr instructionTable[c_INSTRUCTION_TABLE_SIZE]{};
    opcodeFnPtr prefixedInstructionTable[c_INSTRUCTION_TABLE_SIZE]{};
    

    //Operation Types
    word_t jump(bool valid);
    word_t load();



    //Opcodes

    word_t OP_0x00();


    word_t OP_OxCB(); //Prefix Instruction
};



#endif