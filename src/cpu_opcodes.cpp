#include "inc/cpu.h"
#include "inc/bitfuncs.h"

#include <stdexcept>
#include <iostream>
#include <cassert>

void CPU::setupTables()
{
    for(word_t i {0}; i < c_INSTRUCTION_TABLE_SIZE; ++i)
    {
        instructionTable[i] = &CPU::OP_NOT_IMPLEMTED;
        instructionTable2[i] = &CPU::OP_NOT_IMPLEMTED2;
    }

    for (byte_t i {0x40}; i < 0x80; ++i)
    {
        instructionTable2[i] = &CPU::cpu_byteLoad;
    }
    for (byte_t i {0x06}; i <= 0x3E; i+=0x8)
    {
        instructionTable2[i] = &CPU::cpu_byteLoad;
    }
    
    //LD A,n
    instructionTable[0x0A] = &CPU::OP_0x0A;
    instructionTable[0x1A] = &CPU::OP_0x1A;
    instructionTable[0xFA] = &CPU::OP_0xFA;
    //LD n,A
    instructionTable[0x02] = &CPU::OP_0x02;
    instructionTable[0x12] = &CPU::OP_0x12;
    instructionTable[0xEA] = &CPU::OP_0xEA;
    //LD A,(C)
    instructionTable[0xF2] = &CPU::OP_0xF2;
    //LD (C),A
    instructionTable[0xE2] = &CPU::OP_0xE2;
    //LD A,(HL-)
    instructionTable[0x3A] = &CPU::OP_0x3A;
    //LD (HL-),A
    instructionTable[0x32] = &CPU::OP_0x32;
    //LD A,(HL+)
    instructionTable[0x2A] = &CPU::OP_0x2A;
    //LD (HL+),A
    instructionTable[0x22] = &CPU::OP_0x22;
    //LDH (n),A
    instructionTable[0xE0] = &CPU::OP_0xE0;
    //LDH A,(n)
    instructionTable[0xF0] = &CPU::OP_0xF0;
    //WordLoads
    //LD n,nn
    instructionTable[0x01] = &CPU::OP_0x01;
    instructionTable[0x11] = &CPU::OP_0x11;
    instructionTable[0x21] = &CPU::OP_0x21;
    instructionTable[0x31] = &CPU::OP_0x31;
    //LD SP,HL
    instructionTable[0xF9] = &CPU::OP_0xF9;
    //LD HL,SP+n
    //LDHL SP,n
    instructionTable[0xF8] = &CPU::OP_0xF8;
    //LD (nn),SP
    instructionTable[0x08] = &CPU::OP_0x08;
    //PUSH nn
    instructionTable[0xF5] = &CPU::OP_0xF5;
    instructionTable[0xC5] = &CPU::OP_0xC5;
    instructionTable[0xD5] = &CPU::OP_0xD5;
    instructionTable[0xE5] = &CPU::OP_0xE5;
    //POP nn
    instructionTable[0xF1] = &CPU::OP_0xF1;
    instructionTable[0xC1] = &CPU::OP_0xC1;
    instructionTable[0xD1] = &CPU::OP_0xD1;
    instructionTable[0xE1] = &CPU::OP_0xE1;
    
    //Byte arithmetic with reg A
    for (byte_t i{ 0x80 }; i < 0xC0; ++i)
    {
        instructionTable2[i] = &CPU::cpu_byteArithmetic;
    }
    for (byte_t i{0xC6}; i >= 0xC6; i+=0x8)
    {
        instructionTable2[i] = &CPU::cpu_byteArithmetic;
    }

    for (byte_t i{0x04}; i <= 0x3C; i+=0x8)
    {
        instructionTable2[i] = &CPU::cpu_byteInc;
    }
    for (byte_t i{0x05}; i <= 0x3D; i+=0x8)
    {
        instructionTable2[i] = &CPU::cpu_byteDec;
    }
    //Word arithmatic
    //ADD HL,n
    instructionTable[0x09] = &CPU::OP_0x09;
    instructionTable[0x19] = &CPU::OP_0x19;
    instructionTable[0x29] = &CPU::OP_0x29;
    instructionTable[0x39] = &CPU::OP_0x39;
    //ADD SP,n
    instructionTable[0xE8] = &CPU::OP_0xE8;
    //INC nn
    instructionTable[0x03] = &CPU::OP_0x03;
    instructionTable[0x13] = &CPU::OP_0x13;
    instructionTable[0x23] = &CPU::OP_0x23;
    instructionTable[0x33] = &CPU::OP_0x33;
    //DEC nn
    instructionTable[0x0B] = &CPU::OP_0x0B;
    instructionTable[0x1B] = &CPU::OP_0x1B;
    instructionTable[0x2B] = &CPU::OP_0x2B;
    instructionTable[0x3B] = &CPU::OP_0x3B;
    //Jumps
    //JP nn
    instructionTable2[0xC3] = &CPU::cpu_jump;
    //JP cc,nn
    instructionTable2[0xC2] = &CPU::cpu_jump;
    instructionTable2[0xCA] = &CPU::cpu_jump;
    instructionTable2[0xD2] = &CPU::cpu_jump;
    instructionTable2[0xDA] = &CPU::cpu_jump;
    //JP (HL)
    instructionTable[0xE9] = &CPU::OP_0xE9;
    // JR n
    for (byte_t i{0x18}; i <= 0x38; i+=0x8)
    {
        instructionTable2[i] = &CPU::cpu_jumpRelative;
    }
    //Calls
    //Call nn
    instructionTable2[0xCD] = &CPU::cpu_jump;
    //CALL cc,nn
    instructionTable2[0xC4] = &CPU::cpu_jump;
    instructionTable2[0xCC] = &CPU::cpu_jump;
    instructionTable2[0xD4] = &CPU::cpu_jump;
    instructionTable2[0xDC] = &CPU::cpu_jump;
    //Restarts
    //RST n
    for (byte_t i{ 0xC7 }; i >= 0xC7; i+=0x8)
    {
        instructionTable2[i] = &CPU::cpu_restart;
    }
    //Returns
    //RET
    instructionTable2[0xC9] = &CPU::cpu_jump;
    //RET cc
    instructionTable2[0xC0] = &CPU::cpu_jump;
    instructionTable2[0xC8] = &CPU::cpu_jump;
    instructionTable2[0xD0] = &CPU::cpu_jump;
    instructionTable2[0xD8] = &CPU::cpu_jump;
    //RETI
    instructionTable[0xD9] = &CPU::OP_0xD9;
    //Miscellaneous
    //DAA Decimal adjust register A
    instructionTable[0x27] = &CPU::OP_0x27;
    //CPL
    instructionTable[0x2F] = &CPU::OP_0x2F;
    //CCF
    instructionTable[0x3F] = &CPU::OP_0x3F;
    //SCF
    instructionTable[0x37] = &CPU::OP_0x37;
    //NOP
    instructionTable[0x00] = &CPU::OP_0x00;
    //HALT
    instructionTable[0x76] = &CPU::OP_0x76;
    //STOP
    instructionTable[0x10] = &CPU::OP_0x10;
    //DI disable interupts
    instructionTable[0xF3] = &CPU::OP_0xF3;
    //EI enable interupts
    instructionTable[0xFB] = &CPU::OP_0xFB;
    //Roates & Shifts
    //RLCA
    instructionTable[0x07] = &CPU::OP_0x07;
    //RLA
    instructionTable[0x17] = &CPU::OP_0x17;
    //RRCA
    instructionTable[0x0F] = &CPU::OP_0x0F;
    //RRA
    instructionTable[0x1F] = &CPU::OP_0x1F;


    instructionTable[0xD3] = &CPU::OP_ILLEGAL;
    instructionTable[0xDB] = &CPU::OP_ILLEGAL;
    instructionTable[0xDD] = &CPU::OP_ILLEGAL;
    instructionTable[0xE4] = &CPU::OP_ILLEGAL;
    instructionTable[0xE3] = &CPU::OP_ILLEGAL;
    instructionTable[0xEB] = &CPU::OP_ILLEGAL;
    instructionTable[0xEC] = &CPU::OP_ILLEGAL;
    instructionTable[0xED] = &CPU::OP_ILLEGAL;
    instructionTable[0xF3] = &CPU::OP_ILLEGAL;
    instructionTable[0xFD] = &CPU::OP_ILLEGAL;
    instructionTable[0xFC] = &CPU::OP_ILLEGAL;
}

int CPU::OP_NOT_IMPLEMTED() { throw std::runtime_error("OPCODE NOT IMPLEMENTED in table 1!"); }
int CPU::OP_NOT_IMPLEMTED2(const byte_t& opcode) { return -1; }
int CPU::OP_ILLEGAL() { return 4; }

int CPU::opcode_Translator(byte_t opcode)
{
    int cycles{ ((*this).*(instructionTable2[opcode]))(opcode) };

    if (cycles >= 0)
        return cycles;
    else
        return ((*this).*(instructionTable[opcode]))();
     
}

int CPU::CBopcode_Translator(byte_t opcode)
{
    using cbFunctionPtr = void(CPU::*)(byte_t&, int);
    static const std::vector<std::pair<cbFunctionPtr, std::string>> bitFunction
    {
        {&CPU::cpu_leftRotate, "RLC"}, {&CPU::cpu_rightRotate, "RRC"}, 
        {&CPU::cpu_leftRotateWithCarry, "RL"}, {&CPU::cpu_rightRotateWithCarry, "RR"},
        {&CPU::cpu_leftShift, "SLA"}, {&CPU::cpu_rightShiftArithmetic, "SRA"}, 
        {&CPU::cpu_swapNibbles, "SWAP"}, {&CPU::cpu_rightShift, "SRL"},
        {&CPU::cpu_testBit, "BIT"}, {&CPU::cpu_resetBit, "RES"}, {&CPU::cpu_setBit, "SET"}
    };

    int regInt{ extractBits(opcode,0,3) }; 
    int bit{ extractBits(opcode,3,3) };
    int funcInt{ opcode < 0x40 ? extractBits(opcode,3,3) : 7+extractBits(opcode,6,2) };
    assert((funcInt>=0&&funcInt<=10) && "incorrect index in CBopcode_Translator!");

    logOpcode((m_PC-2), 0xCB, opcode, 0x0, bitFunction[funcInt].second, getRegisterStr(regInt), ((opcode >= 0x40) ? std::to_string(bit) : ""));

    if (regInt != 6)
    {
        ((*this).*(bitFunction[funcInt]).first)(getRegister(regInt), bit);
        return 8;
    }
    else
    {
        byte_t HL{ readByte(m_Registers.get_hl()) };
        ((*this).*(bitFunction[funcInt]).first)(HL, bit);
        writeByte(m_Registers.get_hl(), HL);
        return (funcInt!=8) ? 16 : 12;
    }
}

//Specific Commands
//byte Loads
//LD A,n
int CPU::OP_0x0A()
{
    logOpcode((m_PC-1), 0x0A, 0x0, 0x0, "LD", "A", "[BC]");

    m_Registers.a = readByte(m_Registers.get_bc());
    return 8;
}
int CPU::OP_0x1A()
{
    logOpcode((m_PC-1), 0x1A, 0x0, 0x0, "LD", "A", "[DE]");

    m_Registers.a = readByte(m_Registers.get_de());
    return 8;
}
int CPU::OP_0xFA()
{
    word_t address{ readNextWord() };
    logOpcode((m_PC-3), 0xFA, 0x0, 0x0, "LD", "A", "$"+wordStr(address));

    m_Registers.a = readByte(address);
    return 16;
}
//LD n,A
int CPU::OP_0x02()
{
    logOpcode((m_PC-1), 0x02, 0x0, 0x0, "LD", "[BC]", "A");

    writeByte(m_Registers.get_bc(), m_Registers.a);
    return  8;
}
int CPU::OP_0x12()
{
    logOpcode((m_PC-1), 0x12, 0x0, 0x0, "LD", "[DE]", "A");

    writeByte(m_Registers.get_de(), m_Registers.a);
    return  8;
}
int CPU::OP_0xEA()
{
    word_t address{ readNextWord() };
    logOpcode((m_PC-3), 0xEA, 0x0, 0x0, "LD", "$"+wordStr(address), "A");

    writeByte(address, m_Registers.a);
    return  16;
}
//LD A,(C)
int CPU::OP_0xF2()
{
    logOpcode((m_PC-1), 0xF2, 0x0, 0x0, "LD", "A", "$"+wordStr(0xFF00u + m_Registers.c));
    m_Registers.a = readByte(0xFF00u + m_Registers.c);
    return 8;
}
//LD (C),A
int CPU::OP_0xE2()
{
    logOpcode((m_PC-1), 0xE2, 0x0, 0x0, "LD", "$"+wordStr(0xFF00u + m_Registers.c), "A");
    writeByte(0xFF00u + m_Registers.c, m_Registers.a);
    return 8;
}
//LD A,(HL-)
int CPU::OP_0x3A()
{
    logOpcode((m_PC-1), 0x3A, 0x0, 0x0, "LD", "A", "[HL-]");

    word_t HL{ m_Registers.get_hl() };
    m_Registers.a = readByte(HL);
    m_Registers.set_hl( --HL );
    return 8;
}
//LD (HL-),A
int CPU::OP_0x32()
{
    logOpcode((m_PC-1), 0x32, 0x0, 0x0, "LD","[HL-]","A");

    word_t HL{ m_Registers.get_hl() };
    writeByte(HL, m_Registers.a);
    m_Registers.set_hl( --HL );
    return 8;
}
//LD A,(HL+)
int CPU::OP_0x2A()
{
    logOpcode((m_PC-1), 0x2A, 0x0, 0x0, "LD", "A", "[HL+]");

    word_t HL{ m_Registers.get_hl() };
    m_Registers.a = readByte(HL);
    m_Registers.set_hl( ++HL );
    return 8;
}
//LD (HL+),A
int CPU::OP_0x22()
{
    logOpcode((m_PC-1), 0x22, 0x0, 0x0, "LD", "[HL+]", "A");

    word_t HL{ m_Registers.get_hl() };
    writeByte(HL, m_Registers.a);
    m_Registers.set_hl( ++HL );
    return 8;
}
//LDH (n),A
int CPU::OP_0xE0()
{
    byte_t addr { readNextByte() };
    logOpcode((m_PC-2), 0xE0, 0x0, 0x0, "LD", "$"+wordStr(0xFF00u + addr), "A");
    writeByte(0xFF00u + addr, m_Registers.a);
    return 12;
}
//LDH A,(n)
int CPU::OP_0xF0()
{
    byte_t addr { readNextByte() };
    logOpcode((m_PC-2), 0xF0, 0x0, 0x0, "LD", "A", "$"+wordStr(0xFF00u + addr));

    m_Registers.a = readByte(0xFF00u + addr);
    return 12;
}

//*********************************************************//

//WordLoads
//LD n,nn
int CPU::OP_0x01()
{
    word_t value{ readNextWord() };
    logOpcode((m_PC-3), 0x01, 0x0, 0x0, "LD", "BC", wordStr(value));

    m_Registers.set_bc(value);
    return 12;
}
int CPU::OP_0x11()
{
    word_t value{ readNextWord() };
    logOpcode((m_PC-3), 0x11, 0x0, 0x0, "LD", "DE", wordStr(value));

    m_Registers.set_de(value);
    return 12;
}
int CPU::OP_0x21()
{
    word_t value{ readNextWord() };
    logOpcode((m_PC-3), 0x21, 0x0, 0x0, "LD", "HL", wordStr(value));

    m_Registers.set_hl(value);
    return 12;
}
int CPU::OP_0x31()
{
    word_t value{ readNextWord() };
    logOpcode((m_PC-3), 0x31, 0x0, 0x0, "LD", "SP", wordStr(value));

    m_SP = value;
    return 12;
}
//LD SP,HL
int CPU::OP_0xF9()
{
    logOpcode((m_PC-1), 0xF9, 0x0, 0x0, "LD", "SP", "HL");

    m_SP = m_Registers.get_hl();
    return 8;
}
//LD HL,SP+n
//LDHL SP,n
int CPU::OP_0xF8()
{
    byte_t unsignedData{ readNextByte() };
    int ctest{ (m_SP&0x00FF) + unsignedData };
    int htest{ (m_SP&0x000F) + (unsignedData&0x0F) };

    logOpcode((m_PC-2), 0xF8, unsignedData, 0x0, "LD", "HL", "SP+i8");

    m_Registers.set_hl(signedAddition(m_SP, unsignedData));

    m_Registers.f.zero = false;
    m_Registers.f.subtract = false;
    m_Registers.f.carry = ctest >= 0x100;
    m_Registers.f.half_carry = htest >= 0x10;
    return 12;
}
//LD (nn),SP
int CPU::OP_0x08()
{
    word_t address{ readNextWord() };

    logOpcode((m_PC-3), 0x08, 0x0, 0x0, "LD", "$"+wordStr(address), "SP");

    byte_t lsb{ static_cast<byte_t>(m_SP & 0xFF) };
    byte_t msb{ static_cast<byte_t>(m_SP >> 8) };
    
    writeByte(address, lsb);
    writeByte(address+1, msb);

    return 20;
}
//PUSH nn
int CPU::OP_0xF5()
{
    logOpcode((m_PC-1), 0xF5, 0x0, 0x0, "PUSH", "AF", "");
    push(m_Registers.get_af());
    return 16;
}
int CPU::OP_0xC5()
{
    logOpcode((m_PC-1), 0xC5, 0x0, 0x0, "PUSH", "BC", "");
    push(m_Registers.get_bc());
    return 16;
}
int CPU::OP_0xD5()
{
    logOpcode((m_PC-1), 0xD5, 0x0, 0x0, "PUSH", "DE", "");
    push(m_Registers.get_de());
    return 16;
}

int CPU::OP_0xE5()
{
    logOpcode((m_PC-1), 0xE5, 0x0, 0x0, "PUSH", "HL", "");
    push(m_Registers.get_hl());
    return 16;
}
//POP nn
int CPU::OP_0xF1()
{
    logOpcode((m_PC-1), 0xF1, 0x0, 0x0, "POP", "AF", "");
    m_Registers.set_af(pop());
    return 12;
}
int CPU::OP_0xC1()
{
    logOpcode((m_PC-1), 0xC1, 0x0, 0x0, "POP", "BC", "");
    m_Registers.set_bc(pop());
    return 12;
}
int CPU::OP_0xD1()
{
    logOpcode((m_PC-1), 0xD1, 0x0, 0x0, "POP", "DE", "");
    m_Registers.set_de(pop());
    return 12;
}
int CPU::OP_0xE1()
{
    logOpcode((m_PC-1), 0xE1, 0x0, 0x0, "POP", "HL", "");
    m_Registers.set_hl(pop());
    return 12;
}

//Word arithmatic
//ADD HL,n
int CPU::OP_0x09()
{
    logOpcode((m_PC-1), 0x09, 0x0, 0x0, "ADD", "HL", "BC");
    word_t HL{ m_Registers.get_hl() };
    wordAdd(HL, m_Registers.get_bc());
    m_Registers.set_hl(HL);
    return 8;
}
int CPU::OP_0x19()
{
    logOpcode((m_PC-1), 0x19, 0x0, 0x0, "ADD", "HL", "DE");
    word_t HL{ m_Registers.get_hl() };
    wordAdd(HL, m_Registers.get_de());
    m_Registers.set_hl(HL);
    return 8;
}
int CPU::OP_0x29()
{
    logOpcode((m_PC-1), 0x29, 0x0, 0x0, "ADD", "HL", "HL");
    word_t HL{ m_Registers.get_hl() };
    wordAdd(HL, m_Registers.get_hl());
    m_Registers.set_hl(HL);
    return 8;
}
int CPU::OP_0x39()
{
    logOpcode((m_PC-1), 0x39, 0x0, 0x0, "ADD", "HL", "SP");
    word_t HL{ m_Registers.get_hl() };
    wordAdd(HL, m_SP);
    m_Registers.set_hl(HL);
    return 8;
}
//ADD SP,n
int CPU::OP_0xE8()
{
    byte_t unsignedData{ readNextByte() };
    int ctest{ (m_SP&0x00FF) + unsignedData };
    int htest{ (m_SP&0x000F) + (unsignedData&0x0F) };

    logOpcode((m_PC-2), 0xE8, unsignedData, 0x0, "ADD", "SP", "i8");
    m_SP = signedAddition(m_SP, unsignedData);

    m_Registers.f.zero = false;
    m_Registers.f.subtract = false;
    m_Registers.f.carry = ctest >= 0x100;
    m_Registers.f.half_carry = htest >= 0x10;
    return 16;
}
//INC nn
int CPU::OP_0x03()
{
    logOpcode((m_PC-1), 0x03, 0x0, 0x0, "INC", "BC", "");
    m_Registers.set_bc(m_Registers.get_bc() + 1u);
    return 8;
}
int CPU::OP_0x13()
{
    logOpcode((m_PC-1), 0x13, 0x0, 0x0, "INC", "DE", "");
    m_Registers.set_de(m_Registers.get_de() + 1u);
    return 8;
}
int CPU::OP_0x23()
{
    logOpcode((m_PC-1), 0x23, 0x0, 0x0, "INC", "HL", "");
    m_Registers.set_hl(m_Registers.get_hl() + 1u);
    return 8;
}
int CPU::OP_0x33()
{
    logOpcode((m_PC-1), 0x33, 0x0, 0x0, "INC", "SP", "");
    ++m_SP;
    return 8;
}
//DEC nn
int CPU::OP_0x0B()
{
    logOpcode((m_PC-1), 0x0B, 0x0, 0x0, "DEC", "BC", "");
    m_Registers.set_bc(m_Registers.get_bc() - 1u);
    return 8;
}
int CPU::OP_0x1B()
{
    logOpcode((m_PC-1), 0x1B, 0x0, 0x0, "DEC", "DE", "");
    m_Registers.set_de(m_Registers.get_de() - 1u);
    return 8;
}
int CPU::OP_0x2B()
{
    logOpcode((m_PC-1), 0x2B, 0x0, 0x0, "DEC", "HL", "");
    m_Registers.set_hl(m_Registers.get_hl() - 1u);
    return 8;
}
int CPU::OP_0x3B()
{
    logOpcode((m_PC-1), 0x3B, 0x0, 0x0, "DEC", "SP", "");
    --m_SP;
    return 8;
}

//JP HL
int CPU::OP_0xE9()
{
    logOpcode((m_PC-1), 0xE9, 0x0, 0x0, "JP", "HL", "");
    m_PC = m_Registers.get_hl();
    return 4;
}
//RETI
int CPU::OP_0xD9()
{
    logOpcode((m_PC-1), 0xD9, 0x0, 0x0, "RETI", "", "");
    m_PC = pop();
    m_InteruptsEnabled = true;
    return 8;
}
//Miscellaneous
//DAA
int CPU::OP_0x27()
{
    logOpcode((m_PC-1), 0x27, 0x0, 0x0, "DAA", "A", "");
    checkDAA(m_Registers.a);
    return 4;
}
//CPL
int CPU::OP_0x2F()
{
    logOpcode((m_PC-1), 0x2F, 0x0, 0x0, "CPL", "A", "");
    m_Registers.a = ~m_Registers.a;
    m_Registers.f.subtract = true;
    m_Registers.f.half_carry = true;
    return 4;
}
//CCF
int CPU::OP_0x3F()
{
    logOpcode((m_PC-1), 0x3F, 0x0, 0x0, "CCF", "", "");
    m_Registers.f.carry = !m_Registers.f.carry;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = false;
    return 4;
}
//SCF
int CPU::OP_0x37()
{
    logOpcode((m_PC-1), 0x37, 0x0, 0x0, "SCF", "", "");
    m_Registers.f.carry = true;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = false;
    return 4;
}
//NOP
int CPU::OP_0x00()
{
    logOpcode((m_PC-1), 0x0, 0x0, 0x0, "NOP", "", "");
    return 4;
}
/*********************vvvvvv************************/
//HALT
int CPU::OP_0x76()
{
    logOpcode((m_PC-1), 0x76, 0x0, 0x0, "HALT", "", "");
    m_Halted = true;
    return 4;
}
//STOP
int CPU::OP_0x10()
{
    logOpcode((m_PC-1), 0x10, 0x0, 0x0, "STOP", "", "");
    // std::exit(EXIT_FAILURE);
    return 4;
}
/***********************^^^^^^^************************/
//DI disable interupts
int CPU::OP_0xF3()
{
    logOpcode((m_PC-1), 0xF3, 0x0, 0x0, "DI", "", "");
    m_InteruptsEnabled = false;
    return 4;
}
//EI enable interupts
int CPU::OP_0xFB()
{
    logOpcode((m_PC-1), 0xFB, 0x0, 0x0, "EI", "", "");
    m_InteruptsEnabled = true;
    return 4;
}
//Rotates and shifts
//RLCA
int CPU::OP_0x07()
{
    logOpcode((m_PC-1), 0x07, 0x0, 0x0, "RLCA", "", "");
    cpu_leftRotate(m_Registers.a, 0);
    m_Registers.f.zero = false;
    return 4;
}
//RLA
int CPU::OP_0x17()
{
    logOpcode((m_PC-1), 0x17, 0x0, 0x0, "RLA", "", "");
    cpu_leftRotateWithCarry(m_Registers.a, 0);
    m_Registers.f.zero = false;
    return 4;
}
//RRCA
int CPU::OP_0x0F()
{
    logOpcode((m_PC-1), 0x0F, 0x0, 0x0, "RRCA", "", "");
    cpu_rightRotate(m_Registers.a, 0);
    m_Registers.f.zero = false;
    return 4;
}
//RRA
int CPU::OP_0x1F()
{
    logOpcode((m_PC-1), 0x1F, 0x0, 0x0, "RRA", "", "");
    cpu_rightRotateWithCarry(m_Registers.a, 0);
    m_Registers.f.zero = false;
    return 4;
}