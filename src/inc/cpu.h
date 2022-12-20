#ifndef H_CPU
#define H_CPU

#include "consts.h"
#include "registers.h"
#include "memoryBus.h"
#include "platform.h"
#include "BSlogger.h"
#include "ppu.h"
#include "apu.h"

#include <functional>

class CPU
{
private:
    Registers  m_Registers{};
    word_t     m_PC{};
    word_t     m_SP{};

    logger&    m_log;
    MemoryBus& m_Memory;
    Platform&  m_Platform;
    PPU&       m_PPU;
    APU&       m_APU;
    Settings&  m_Settings;
    
    int  m_TimerCounter{}; //Tracks when to throw timer interupts
    int  m_DividerCounter{}; //Tracks when to increment Divider Register
    bool m_InteruptsEnabled{}; // IME
    int  m_IMEScheduled{}; // 0 none, 1 scheldued for next cycle, 2 enable interupts
    bool m_Halted{};
    bool m_haltBug{};
    bool m_Stopped{};
    
    using opcodeFnPtr = std::function<int(void)>;
    using opcodeFnPtr2 = std::function<int(byte_t)>;
    opcodeFnPtr instructionTable[c_INSTRUCTION_TABLE_SIZE]{};
    opcodeFnPtr2 instructionTable2[c_INSTRUCTION_TABLE_SIZE]{};

public:
    CPU(MemoryBus& memoryRef, logger& logRef, Platform& platformRef, PPU& ppuRef, APU& apuRef, Settings& settingsRef);
    int  cycle();
    void update(int cycles);
    void updateJoypad();
    int  interupts();
    bool isHalted();
    bool isStopped();

private:
    int    execute(byte_t instructionByte, bool prefixed);
    void   setupTables();
    word_t readNextWord();
    byte_t readNextByte();
    byte_t readAudioRegister(word_t address) const;
    void   writeAudioRegister(word_t address, byte_t value);
    byte_t readByte(word_t address) const;
    void   writeByte(word_t address, byte_t value);
    
    void   updateTimers(int cycles);
    void   updateDividerRegister(int cycles);
    bool   isClockEnabled() const;
    void   updateClockFreq();
    byte_t getClockFreq() const;
    
    void   keyDown(int key);
    void   keyUp(int key);
    
    void   requestInterupt(int interupt);
    void   performInterupt(int interupt);

    void   logOpcode(word_t PC, byte_t opcode, byte_t arg1, byte_t arg2, std::string_view func, std::string_view peram1, std::string_view peram2) const;
    void   initiateDMATransfer(byte_t value);

    //Opcode Commands
    void   push(word_t value);
    word_t pop();

    byte_t&          getRegister(int index);
    std::string_view getRegisterStr(int index);
    std::string      byteStr(byte_t byte);
    std::string      wordStr(word_t word);
    
    int opcode_Translator(byte_t opcode);
    int cpu_restart(byte_t opcode);
    int cpu_byteLoad(byte_t opcode);

    int cpu_jump(byte_t opcode);
    enum class JumpTest 
    {
        NotZero, Zero, NotCarry, Carry, Always,  
    };
    std::string_view getJumpTestStr(int type);
    bool testJumpTest(JumpTest type);
    int  jump(int type, byte_t opcode);
    int  call(int type, byte_t opcode);
    int  return_(int type, byte_t opcode);

    int    cpu_jumpRelative(byte_t opcode);
    word_t signedAddition(word_t target, byte_t unsignedData);

    int  cpu_byteArithmetic(byte_t opcode);
    void byteAdd(byte_t data);
    void byteAddWithCarry(byte_t data);
    void byteSub(byte_t data);
    void byteSubWithCarry(byte_t data);
    void byteAND(byte_t data);
    void byteOR(byte_t data);
    void byteXOR(byte_t data);
    void byteCP(byte_t data); //compare
    void checkDAA(byte_t& byte);

    int  cpu_byteInc(byte_t opcode);
    int  cpu_byteDec(byte_t opcode);
    void byteINC(byte_t& target); 
    void byteDEC(byte_t& target); 

    void wordAdd(word_t& reg, word_t addValue);

    //CB commands
    int  CBopcode_Translator(byte_t opcode);
    
    void cpu_testBit(byte_t& byte, int bit);
    void cpu_resetBit(byte_t& byte, int bit);
    void cpu_setBit(byte_t& byte, int bit);
    void cpu_swapNibbles(byte_t& reg, int);
    void cpu_leftRotate(byte_t& byte, int);
    void cpu_leftRotateWithCarry(byte_t& byte, int);
    void cpu_rightRotate(byte_t& byte, int);
    void cpu_rightRotateWithCarry(byte_t& byte, int);
    void cpu_leftShift(byte_t& byte, int);
    void cpu_rightShift(byte_t& byte, int);
    void cpu_rightShiftArithmetic(byte_t& byte, int);

    int  OP_NOT_IMPLEMTED();
    int  OP_ILLEGAL();
    int  OP_NOT_IMPLEMTED2(byte_t);
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
    //JP (HL)
    int OP_0xE9();
    //Returns
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