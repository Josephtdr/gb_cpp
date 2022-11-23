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

    using opcodeFnPtr = int(CPU::*)();
    opcodeFnPtr instructionTable[c_INSTRUCTION_TABLE_SIZE]{};
    opcodeFnPtr prefixedInstructionTable[c_INSTRUCTION_TABLE_SIZE]{};
    
    word_t readNextWord();
    byte_t readNextByte();
    
    void push(word_t value);
    word_t pop();
    
    void noOperation();
    void halt();

    //Jumps
    enum class JumpTest 
    {
        NotZero,
        Zero,
        NotCarry,
        Carry,
        Always
    };
    bool testJumpTest(JumpTest type);
    void jump(JumpTest type, const word_t& address);
    void call(JumpTest type, const word_t& address);
    void return_(JumpTest type);
    void restart(byte_t address);

    //Loads
    enum class ByteLoadTarget
    {
        A, B, C, D, E, H, L, D16I, BCI, DEI, HLI, FF00pC, FF00pD8,
    };
    enum class ByteLoadSource
    {
        A, B, C, D, E, H, L, D8, D16I, BCI, DEI, HLI, FF00pC, FF00pD8,
    };
    void byteLoad(ByteLoadTarget ldTarget, ByteLoadSource ldSource);
    
    enum class WordLoadTarget
    {
        BC, DE, HL, SP, D16I,
    };
    enum class WordLoadSource
    {
        HL, SP, D16, SPpD8,
    };
    void wordLoad(WordLoadTarget ldTarget, WordLoadSource ldSource);

    //Byte Arithmetic
    void byteAdd(byte_t& reg, const byte_t& addValue, bool withCarry=false);
    void byteSub(byte_t& reg, const byte_t& subValue, bool withCarry=false);
    void byteAND(byte_t& reg, const byte_t& andValue);
    void byteOR(byte_t& reg, const byte_t& orValue);
    void byteXOR(byte_t& reg, const byte_t& xorValue);
    void byteCP(const byte_t& reg, const byte_t& cmpValue); //compare
    void byteINC(byte_t& target); //increment
    void byteDEC(byte_t& target); //decrement
    //Word Arithmetic
    void wordAdd(word_t& reg, const word_t& addValue);
    

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

    //WordLoads
    //LD n,nn
    int OP_0x01();
    int OP_0x11();
    int OP_0x21();
    int OP_0x31();
    //LD SP,HL
    int OP_0xF9();
    //LD HL,SP+n
    //LDHL SP,n
    int OP_0xF8();
    //LD (nn),SP
    int OP_0x08();
    //PUSH nn
    int OP_0xF5();
    int OP_0xC5();
    int OP_0xD5();
    int OP_0xE5();
    //POP nn
    int OP_0xF1();
    int OP_0xC1();
    int OP_0xD1();
    int OP_0xE1();
    
    //Byte arithmetic with reg A
    //ADD A,n
    int OP_0x87();
    int OP_0x80();
    int OP_0x81();
    int OP_0x82();
    int OP_0x83();
    int OP_0x84();
    int OP_0x85();
    int OP_0x86();
    int OP_0xC6();
    //ADC A,n
    int OP_0x8F();
    int OP_0x88();
    int OP_0x89();
    int OP_0x8A();
    int OP_0x8B();
    int OP_0x8C();
    int OP_0x8D();
    int OP_0x8E();
    int OP_0xCE();
    //SUB A, n
    int OP_0x97();
    int OP_0x90();
    int OP_0x91();
    int OP_0x92();
    int OP_0x93();
    int OP_0x94();
    int OP_0x95();
    int OP_0x96();
    int OP_0xD6();
    //SBC A,n
    int OP_0x9F();
    int OP_0x98();
    int OP_0x99();
    int OP_0x9A();
    int OP_0x9B();
    int OP_0x9C();
    int OP_0x9D();
    int OP_0x9E();
    // int OP_0x??(); rip D8
    //AND A, n
    int OP_0xA7();
    int OP_0xA0();
    int OP_0xA1();
    int OP_0xA2();
    int OP_0xA3();
    int OP_0xA4();
    int OP_0xA5();
    int OP_0xA6();
    int OP_0xE6();
    //OR A, n
    int OP_0xB7();
    int OP_0xB0();
    int OP_0xB1();
    int OP_0xB2();
    int OP_0xB3();
    int OP_0xB4();
    int OP_0xB5();
    int OP_0xB6();
    int OP_0xF6();
    //XOR A, n
    int OP_0xAF();
    int OP_0xA8();
    int OP_0xA9();
    int OP_0xAA();
    int OP_0xAB();
    int OP_0xAC();
    int OP_0xAD();
    int OP_0xAE();
    int OP_0xEE();
    //CP A, n
    int OP_0xBF();
    int OP_0xB8();
    int OP_0xB9();
    int OP_0xBA();
    int OP_0xBB();
    int OP_0xBC();
    int OP_0xBD();
    int OP_0xBE();
    int OP_0xFE();
    //INC n
    int OP_0x3C();
    int OP_0x04();
    int OP_0x0C();
    int OP_0x14();
    int OP_0x1C();
    int OP_0x24();
    int OP_0x2C();
    int OP_0x34();
    //DEC n
    int OP_0x3D();
    int OP_0x05();
    int OP_0x0D();
    int OP_0x15();
    int OP_0x1D();
    int OP_0x25();
    int OP_0x2D();
    int OP_0x35();

    //Word arithmatic
    //ADD HL,n
    int OP_0x09();
    int OP_0x19();
    int OP_0x29();
    int OP_0x39();
    //ADD SP,n
    int OP_0xE8();
    //INC nn
    int OP_0x03();
    int OP_0x13();
    int OP_0x23();
    int OP_0x33();
    //DEC nn
    int OP_0x0B();
    int OP_0x1B();
    int OP_0x2B();
    int OP_0x3B();

    //Jumps
    //JP nn
    int OP_0xC3();
    //JP cc,nn
    int OP_0xC2();
    int OP_0xCA();
    int OP_0xD2();
    int OP_0xDA();
    //JP (HL)
    int OP_0xE9();
    // JR n
    int OP_0x18();
    //JR cc,n
    int OP_0x20();
    int OP_0x28();
    int OP_0x30();
    int OP_0x38();
    //Calls
    //Call nn
    int OP_0xCD();
    //CALL cc,nn
    int OP_0xC4();
    int OP_0xCC();
    int OP_0xD4();
    int OP_0xDC();
    //Restarts
    //RST n
    int OP_0xC7();
    int OP_0xCF();
    int OP_0xD7();
    int OP_0xDF();
    int OP_0xE7();
    int OP_0xEF();
    int OP_0xF7();
    int OP_0xFF();
    //Returns
    //RET
    int OP_0xC9();
    //RET cc
    int OP_0xC0();
    int OP_0xC8();
    int OP_0xD0();
    int OP_0xD8();
    //RETI
    int OP_0xD9();
    

    int OP_OxCB(); //Prefix Instruction
};



#endif