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
        A, B, C, D, E, H, L, D16I, BCI, DEI, HLI, FF00pC, FF00pD8,
    };
    enum class ByteLoadSource
    {
        A, B, C, D, E, H, L, D8, D16I, BCI, DEI, HLI, FF00pC, FF00pD8,
    };
    void byteLoad(ByteLoadTarget ldTarget, ByteLoadSource ldSource);

    word_t wordLoad();
    //Other loads---
    //AFromIndirect
    //IndirectFromA
    //AFromByteAddress
    //ByteAddressFromA


    //Opcodes
    //byte Loads
    //LD nn,n
    int OP_0x06();
    int OP_0x0E();
    int OP_0x16();
    int OP_0x1E();
    int OP_0x26();
    int OP_0x2E();
    //LD r1, r2
    int OP_0x7F();
    int OP_0x78();
    int OP_0x79();
    int OP_0x7A();
    int OP_0x7B();
    int OP_0x7C();
    int OP_0x7D();
    int OP_0x7E();
    int OP_0x40();
    int OP_0x41();
    int OP_0x42();
    int OP_0x43();
    int OP_0x44();
    int OP_0x45();
    int OP_0x46();
    int OP_0x48();
    int OP_0x49();
    int OP_0x4A();
    int OP_0x4B();
    int OP_0x4C();
    int OP_0x4D();
    int OP_0x4E();
    int OP_0x50();
    int OP_0x51();
    int OP_0x52();
    int OP_0x53();
    int OP_0x54();
    int OP_0x55();
    int OP_0x56();
    int OP_0x58();
    int OP_0x59();
    int OP_0x5A();
    int OP_0x5B();
    int OP_0x5C();
    int OP_0x5D();
    int OP_0x5E();
    int OP_0x60();
    int OP_0x61();
    int OP_0x62();
    int OP_0x63();
    int OP_0x64();
    int OP_0x65();
    int OP_0x66();
    int OP_0x68();
    int OP_0x69();
    int OP_0x6A();
    int OP_0x6B();
    int OP_0x6C();
    int OP_0x6D();
    int OP_0x6E();
    int OP_0x70();
    int OP_0x71();
    int OP_0x72();
    int OP_0x73();
    int OP_0x74();
    int OP_0x75();
    int OP_0x36();
    //LD A,n
    int OP_0x0A();
    int OP_0x1A();
    int OP_0xFA();
    int OP_0x3E();
    //LD n,A
    int OP_0x47();
    int OP_0x4F();
    int OP_0x57();
    int OP_0x5F();
    int OP_0x67();
    int OP_0x6F();
    int OP_0x02();
    int OP_0x12();
    int OP_0x77();
    int OP_0xEA();
    //LD A,(C)
    int OP_0xF2();
    //LD (C),A
    int OP_0xE2();
    //LD A,(HLD)
    //LD A,(HL-)
    //LDD A,(HL)
    int OP_0x3A();
    //LD (HLD),A
    //LD (HL-),A
    //LDD (HL),A
    int OP_0x32();
    //LD A,(HLI)
    //LD A,(HL+)
    //LDI A,(HL)
    int OP_0x2A();
    //LD (HLI),A
    //LD (HL+),A
    //LDI (HL),A
    int OP_0x22();
    //LDH (n),A
    int OP_0xE0();
    //LDH A,(n)
    int OP_0xF0();

    //////////
    int OP_0x00();

    //nn Pop
    int OP_OxF1(); //POP AF
    int OP_OxC1(); //POP BC
    int OP_OxD1(); //POP DE
    int OP_OxE1(); //POP HL

    int OP_OxCB(); //Prefix Instruction
};



#endif