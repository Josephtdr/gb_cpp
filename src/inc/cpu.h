#ifndef H_CPU
#define H_CPU

#include "consts.h"
#include "registers.h"
#include "memoryBus.h"
#include "BSlogger.h"

class CPU
{
private:
    int m_TimerCounter{};
    int m_DividerCounter{};
    logger m_log;
    MemoryBus m_Memory{m_TimerCounter, m_log};
    Registers m_Registers{};
    word_t m_PC{};
    word_t m_SP{};
    
    bool m_InteruptsEnabled{};
    bool halted{};
    bool m_lineByLine{};

    using opcodeFnPtr = int(CPU::*)();
    using opcodeFnPtr2 = int(CPU::*)(const byte_t&);
    using preCB0x40_FunctionPtr = void(CPU::*)(byte_t&);
    using postCB0x40_FunctionPtr = void(CPU::*)(byte_t&, int);

    opcodeFnPtr instructionTable[c_INSTRUCTION_TABLE_SIZE]{};
    opcodeFnPtr2 instructionTable2[c_INSTRUCTION_TABLE_SIZE]{};

    preCB0x40_FunctionPtr preCB0x40_FunctionTable[(0x40)]{};
    postCB0x40_FunctionPtr postCB0x40_FunctionTable[(0xFF-0x40)]{};

public:
    CPU();
    void frameUpdate();
    void loadGame(const char* fileName);

private:
    int cycle();
    int execute(byte_t instructionByte, bool prefixed);
    void setupTables();
    word_t readNextWord();
    byte_t readNextByte();

    void updateDividerRegister(int cycles);
    void updateTimers(int cycles);
    bool isClockEnabled() const;
    void setClockFreq();
    byte_t getClockFreq() const;
    
    void interupts();
    void requestInterupt(int interupt);
    void performInterupt(int interupt);
    
//******************************************************************************************//
    void push(word_t value);
    word_t pop();

    byte_t& getRegister(int index);
    int opcode_Translator(byte_t opcode);
    int cpu_restart(const byte_t& opcode);
    int cpu_byteLoad(const byte_t& opcode);

    int cpu_byteArithmetic(const byte_t& opcode);
    void byteAdd(const byte_t& data);
    void byteAddWithCarry(const byte_t& data);
    void byteSub(const byte_t& data);
    void byteSubWithCarry(const byte_t& data);
    void byteAND(const byte_t& data);
    void byteOR(const byte_t& data);
    void byteXOR(const byte_t& data);
    void byteCP(const byte_t& data); //compare

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
    void jumpRelative(JumpTest type, const byte_t& unsignedData);
    void call(JumpTest type, const word_t& address);
    void return_(JumpTest type);
    void restart(byte_t address);
    void checkDAA(byte_t& byte);
    void byteINC(byte_t& target); //increment
    void byteDEC(byte_t& target); //decrement
    
    //Word Arithmetic
    void wordAdd(word_t& reg, const word_t& addValue);

    //CB commands
    int CBopcode_Translator(byte_t opcode);
    
    bool testBit(const byte_t& byte, int bit) const;
    void testBit_OP(byte_t& byte, int bit);
    void resetBit(byte_t& byte, int bit);
    void setBit(byte_t& byte, int bit);
    void swapNibbles(byte_t& reg);
    void leftRotate(byte_t& byte);
    void leftRotateWithCarry(byte_t& byte);
    void rightRotate(byte_t& byte);
    void rightRotateWithCarry(byte_t& byte);
    void leftShift(byte_t& byte);
    void rightShift(byte_t& byte);
    void rightShiftArithmetic(byte_t& byte);
    

    int OP_NOT_IMPLEMTED();
    int OP_NOT_IMPLEMTED2(const byte_t&);
    //Unique Opcodes
    //byte Loads
    //LD A,n
    int OP_0x0A();
    int OP_0x1A();
    int OP_0xFA();
    //LD n,A
    int OP_0x02();
    int OP_0x12();
    int OP_0xEA();
    //LD A,(C)
    int OP_0xF2();
    //LD (C),A
    int OP_0xE2();
    //LD A,(HL-)
    int OP_0x3A();
    //LD (HL-),A
    int OP_0x32();
    //LD A,(HL+)
    int OP_0x2A();
    //LD (HL+),A
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
    
    //Miscellaneous
    //DAA Decimal adjust register A
    int OP_0x27();
    //CPL
    int OP_0x2F();
    //CCF
    int OP_0x3F();
    //SCF
    int OP_0x37();
    //NOP
    int OP_0x00();
    //HALT
    int OP_0x76();
    //STOP
    int OP_0x10();
    //DI disable interupts
    int OP_0xF3();
    //EI enable interupts
    int OP_0xFB();
    //Roates & Shifts
    //RLCA
    int OP_0x07();
    //RLA
    int OP_0x17();
    //RRCA
    int OP_0x0F();
    //RRA
    int OP_0x1F();
};



#endif