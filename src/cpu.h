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
    bool halted{};
    
public:
    CPU();
    int cycle();


private:
    int execute(byte_t instructionByte, bool prefixed);

    using opcodeFnPtr = word_t(CPU::*)();
    opcodeFnPtr instructionTable[c_INSTRUCTION_TABLE_SIZE]{};
    opcodeFnPtr prefixedInstructionTable[c_INSTRUCTION_TABLE_SIZE]{};
    
    word_t readNextWord();
    byte_t readNextByte();

    
    void push(word_t value);
    word_t pop();
    word_t call(bool valid);
    word_t return_(bool valid);
    void noOperation();
    void halt();

    enum class JumpTest 
    {
        NotZero,
        Zero,
        NotCarry,
        Carry,
        Always
    };
    word_t jump(JumpTest type);





    enum class ByteLoadTarget
    {
        A, B, C, D, E, H, L, HLI
    };
    enum class ByteLoadSource
    {
        A, B, C, D, E, H, L, D8, HLI
    };
    void byteLoad(ByteLoadTarget ldTarget, ByteLoadSource ldSource);

    word_t wordLoad();
    //Other loads---
    //AFromIndirect
    //IndirectFromA
    //AFromByteAddress
    //ByteAddressFromA


    //Opcodes

    int OP_0x00();

    int OP_OxF1(); //POP AF
    int OP_OxC1(); //POP BC
    int OP_OxD1(); //POP DE
    int OP_OxE1(); //POP HL

    int OP_OxCB(); //Prefix Instruction
};



#endif