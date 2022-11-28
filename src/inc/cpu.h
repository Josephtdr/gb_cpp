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

    using opcodeFnPtr = int(CPU::*)(const byte_t&);
    using preCB0x40_FunctionPtr = void(CPU::*)(byte_t&);
    using postCB0x40_FunctionPtr = void(CPU::*)(byte_t&, int);

    opcodeFnPtr instructionTable[c_INSTRUCTION_TABLE_SIZE]{};
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
    void cpu_log(const byte_t& opcode, std::string_view desc, bool prefixed);
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
    

    byte_t& getRegister(int index);
    int OP_NOT_IMPLEMTED(const byte_t&);
    //General Commands
    int cpu_byteLoad(const byte_t& opcode);
    int cpu_wordLoad(const byte_t& opcode);
    int cpu_byteIndexLoad(const byte_t& opcode);
    int cpu_byteArithmetic(const byte_t& opcode);
    int cpu_wordAdd(const byte_t& opcode);
    int cpu_WordIncDec(const byte_t& opcode);
    int cpu_byteIncDec(const byte_t& opcode);
    int cpu_restart(const byte_t& opcode);
    int cpu_jump(const byte_t& opcode);
    int cpu_jumpRelative(const byte_t& opcode);
    int cpu_wordPopPush(const byte_t& opcode);

    //subfunctions
    void byteAdd(const byte_t& data);
    void wordAdd(word_t& reg, const word_t& data);
    void byteAddWithCarry(const byte_t& data);
    void byteSub(const byte_t& data);
    void byteSubWithCarry(const byte_t& data);
    void byteAND(const byte_t& data);
    void byteOR(const byte_t& data);
    void byteXOR(const byte_t& data);
    void byteCP(const byte_t& data); //compare
    void byteINC(byte_t& reg); 
    void byteDEC(byte_t& reg); 
    bool testJumpTest(int type);
    int call(int type);
    int return_(int type);
    int jump(int type);
    void push(word_t value);
    word_t pop();
    void popAF();

    //More Unique Commands
    int cpu_checkDAA(const byte_t& opcode);
    int cpu_RETI(const byte_t&);
    int cpu_jumpHL(const byte_t&);
    int cpu_CPL(const byte_t&);
    int cpu_CCF(const byte_t&);
    int cpu_SCF(const byte_t&);
    int cpu_NOP(const byte_t&);
    int cpu_halt(const byte_t&);
    int cpu_stop(const byte_t&);
    int cpu_DI(const byte_t&);
    int cpu_EI(const byte_t&);
    int cpu_RLCA(const byte_t&);
    int cpu_RLA(const byte_t&);
    int cpu_RRCA(const byte_t&);
    int cpu_RRA(const byte_t&);
    int cpu_LDd8SP(const byte_t&);
    int cpu_ADDSPr8(const byte_t&);
    int cpu_LDAd16(const byte_t&);
    int cpu_LDd16A(const byte_t&);
    int cpu_0xF2(const byte_t&); //LD A,(0xFF00 + C)
    int cpu_0xE2(const byte_t&); //LD (0xFF00 + C),A
    int cpu_0xE0(const byte_t&); //LDH (0xFF00 + d8),A
    int cpu_0xF0(const byte_t&); //LD  A,(0xFF00 + d8)
    int cpu_0xF8(const byte_t&); //LD HL,SP+i8

    //CB Commands
    int cpu_CB_opcode_Translator(byte_t opcode);
    //subfunctions
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
};



#endif