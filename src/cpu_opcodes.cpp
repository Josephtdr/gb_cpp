#include "inc/cpu.h"

#include <stdexcept>
#include <iostream>

void CPU::setupTables()
{
    for(byte_t i = 0; i < c_INSTRUCTION_TABLE_SIZE; ++i)
    {
        instructionTable[i] = &CPU::OP_NOT_IMPLEMTED;
        instructionTable2[i] = &CPU::OP_NOT_IMPLEMTED2;
    }


    for (byte_t i{ 0x40 }; i < 0x80; ++i)
    {
        instructionTable2[i] = &CPU::cpu_byteLoad;
    }
    for (byte_t i{ 0x06 }; i <= 0x3E; i+=0x8)
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
    instructionTable[0xC3] = &CPU::OP_0xC3;
    //JP cc,nn
    instructionTable[0xC2] = &CPU::OP_0xC2;
    instructionTable[0xCA] = &CPU::OP_0xCA;
    instructionTable[0xD2] = &CPU::OP_0xD2;
    instructionTable[0xDA] = &CPU::OP_0xDA;
    //JP (HL)
    instructionTable[0xE9] = &CPU::OP_0xE9;
    // JR n
    for (byte_t i{0x18}; i <= 0x38; i+=0x8)
    {
        instructionTable2[i] = &CPU::cpu_jumpRelative;
    }
    //Calls
    //Call nn
    instructionTable[0xCD] = &CPU::OP_0xCD;
    //CALL cc,nn
    instructionTable[0xC4] = &CPU::OP_0xC4;
    instructionTable[0xCC] = &CPU::OP_0xCC;
    instructionTable[0xD4] = &CPU::OP_0xD4;
    instructionTable[0xDC] = &CPU::OP_0xDC;
    //Restarts
    //RST n
    for (byte_t i{ 0xC7 }; i >= 0xC7; i+=0x8)
    {
        instructionTable2[i] = &CPU::cpu_restart;
    }
    //Returns
    //RET
    instructionTable[0xC9] = &CPU::OP_0xC9;
    //RET cc
    instructionTable[0xC0] = &CPU::OP_0xC0;
    instructionTable[0xC8] = &CPU::OP_0xC8;
    instructionTable[0xD0] = &CPU::OP_0xD0;
    instructionTable[0xD8] = &CPU::OP_0xD8;
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
}

int CPU::OP_NOT_IMPLEMTED()
{
    throw std::runtime_error("OPCODE NOT IMPLEMENTED in table 1!");
}

int CPU::OP_NOT_IMPLEMTED2(const byte_t& opcode)
{
    std::cout << +opcode << "\n";
    throw std::runtime_error("OPCODE NOT IMPLEMENTED in table 2!");
}


int CPU::opcode_Translator(byte_t opcode)
{
    if (
        (opcode>=0x80 && opcode<0xC0) || //arithmetic
        (opcode>=0x80 && (opcode%8)==6) || //arithmetic
        (opcode>=0x40 && opcode<0x80 && opcode!=0x76) || //byteLoad
        (opcode<0x40 && (opcode%8)==6) || //byteLoad
        (opcode>=0xC0 && (opcode%8)==7) || //rst
        (opcode>=0x18 && opcode <= 0x38 && (opcode%8)==0) ||// Jr
        (opcode>=0x04 && opcode <=0x3D && ((opcode%8)==4 || (opcode%8)==5)) // Inc, Dec
    ) 
    {
        return ((*this).*(instructionTable2[opcode]))(opcode);
    }
    else
    {
        m_log(LOG_INFO) << "PC: " << +(m_PC) << ", Opcode: 0x" 
                        << +opcode  << " \n";
        return ((*this).*(instructionTable[opcode]))();
    }
}

int CPU::CBopcode_Translator(byte_t opcode)
{
    using cbFunctionPtr = void(CPU::*)(byte_t&, int);
    static const std::vector<std::pair<cbFunctionPtr, std::string>> bitFunction
    {
        {&CPU::leftRotate, "RLC"}, {&CPU::rightRotate, "RRC"}, 
        {&CPU::leftRotateWithCarry, "RL"}, {&CPU::rightRotateWithCarry, "RR"},
        {&CPU::leftShift, "SLA"}, {&CPU::rightShiftArithmetic, "SRA"}, 
        {&CPU::swapNibbles, "SWAP"}, {&CPU::rightShift, "SRL"},
        {&CPU::testBit_OP, "BIT"}, {&CPU::resetBit, "RES"}, {&CPU::setBit, "SET"}
    };

    int regIdx{ opcode%8 }; 
    int funcIdx{ opcode < 0x40 ? opcode/8 : 8+((opcode-0x40)/0x40) };
    int bit{ (opcode % 0x40) / 8 };

    m_log(LOG_INFO) << "PC: " << +m_PC << ", Opcode: CB 0x" << +opcode << ", "
                    << bitFunction[funcIdx].second << " "
                    << ((opcode >= 0x40) ? std::to_string(bit) : "") << " "  
                    << getRegisterStr(regIdx) << "\n";

    if (regIdx != 6)
    {
        byte_t& reg = getRegister(opcode%8);
        ((*this).*(bitFunction[funcIdx]).first)(reg, bit);
        return 8;
    }
    else
    {
        byte_t HL{ m_Memory.readByte(m_Registers.get_hl()) };
        ((*this).*(bitFunction[funcIdx]).first)(HL, bit);
        m_Memory.writeByte(m_Registers.get_hl(), HL);
        return (funcIdx!=8) ? 16 : 12;
    }
}

//Specific Commands
//byte Loads
//LD A,n
int CPU::OP_0x0A()
{
    m_Registers.a = m_Memory.readByte(m_Registers.get_bc());
    return 8;
}
int CPU::OP_0x1A()
{
    m_Registers.a = m_Memory.readByte(m_Registers.get_de());
    return 8;
}
int CPU::OP_0xFA()
{
    m_Registers.a = m_Memory.readByte(readNextWord());
    return 16;
}
//LD n,A
int CPU::OP_0x02()
{
    m_Memory.writeByte(m_Registers.get_bc(), m_Registers.a);
    return  8;
}
int CPU::OP_0x12()
{
    m_Memory.writeByte(m_Registers.get_de(), m_Registers.a);
    return  8;
}
int CPU::OP_0xEA()
{
    m_Memory.writeByte(readNextWord(), m_Registers.a);
    return  16;
}
//LD A,(C)
int CPU::OP_0xF2()
{
    m_Registers.a = m_Memory.readByte(0xFF00u + m_Registers.c);
    return 8;
}
//LD (C),A
int CPU::OP_0xE2()
{
    m_Memory.writeByte(0xFF00u + m_Registers.c, m_Registers.a);
    return 8;
}
//LD A,(HL-)
int CPU::OP_0x3A()
{
    m_Registers.a = m_Memory.readByte(m_Registers.get_hl());
    m_Registers.set_hl( m_Registers.get_hl()-1u );
    return 8;
}
//LD (HL-),A
int CPU::OP_0x32()
{
    m_Memory.writeByte(m_Registers.get_hl(), m_Registers.a);
    m_Registers.set_hl( m_Registers.get_hl()-1u );
    return 8;
}
//LD A,(HL+)
int CPU::OP_0x2A()
{
    m_Registers.a = m_Memory.readByte(m_Registers.get_hl());
    m_Registers.set_hl( m_Registers.get_hl()+1u );
    return 8;
}
//LD (HL+),A
int CPU::OP_0x22()
{
    m_Memory.writeByte(m_Registers.get_hl(), m_Registers.a);
    m_Registers.set_hl( m_Registers.get_hl()+1u );
    return 8;
}
//LDH (n),A
int CPU::OP_0xE0()
{
    m_Memory.writeByte(0xFF00u + readNextByte(), m_Registers.a);
    return 12;
}
//LDH A,(n)
int CPU::OP_0xF0()
{
    m_Registers.a = m_Memory.readByte(0xFF00u + readNextByte());
    return 12;
}

//*********************************************************//

//WordLoads
//LD n,nn
int CPU::OP_0x01()
{
    m_Registers.set_bc(readNextWord());
    return 12;
}
int CPU::OP_0x11()
{
    m_Registers.set_de(readNextWord());
    return 12;
}
int CPU::OP_0x21()
{
    m_Registers.set_hl(readNextWord());
    return 12;
}
int CPU::OP_0x31()
{
    m_SP = readNextWord();
    return 12;
}
//LD SP,HL
int CPU::OP_0xF9()
{
    m_SP = m_Registers.get_hl();
    return 8;
}
//LD HL,SP+n
//LDHL SP,n
int CPU::OP_0xF8()
{
    m_log(LOG_ERROR) << "opcode 0xF8 needs to have correct registers!" << "\n";

    byte_t usignedValue{ readNextByte() };
    word_t sum{ unsignedAddition(m_SP, usignedValue) };
    m_Registers.set_hl(sum);

    std::exit(EXIT_FAILURE);

    m_Registers.f.zero = false;
    m_Registers.f.subtract = false;

    // unsigned int v{ static_cast<unsigned int>(m_SP)+static_cast<unsigned int>(d8) };
    // bool carry{ v > 0xFFFF };
    // m_Registers.f.carry = carry;

    // bool half_carry{ ((m_SP & 0xF) + (d8 & 0xF)) > 0xF };
    // m_Registers.f.half_carry = half_carry;

    // value = m_SP+d8; 

    return 12;
}
//LD (nn),SP
int CPU::OP_0x08()
{
    word_t address{ readNextWord() };

    byte_t lsb{ static_cast<byte_t>(m_SP & 0xF) };
    byte_t msb{ static_cast<byte_t>(m_SP >> 4) };
    
    m_Memory.writeByte(address, lsb);
    m_Memory.writeByte(address+1, msb);

    return 20;
}
//PUSH nn
int CPU::OP_0xF5()
{
    push(m_Registers.get_af());
    return 16;
}
int CPU::OP_0xC5()
{
    push(m_Registers.get_bc());
    return 16;
}
int CPU::OP_0xD5()
{
    push(m_Registers.get_de());
    return 16;
}
int CPU::OP_0xE5()
{
    push(m_Registers.get_hl());
    return 16;
}
//POP nn
int CPU::OP_0xF1()
{
    m_log(LOG_ERROR) << "Opcode 0xF1 needs to affect flags somehow" << "\n";
    std::exit(EXIT_FAILURE);

    word_t data{ pop() };
    
    m_Registers.f.zero = !data;
    // m_Registers.f.subtract
    // m_Registers.f.half_carry
    // m_Registers.f.carry

    m_Registers.set_af(data);
    return 12;
}
int CPU::OP_0xC1()
{
    m_Registers.set_bc(pop());
    return 12;
}
int CPU::OP_0xD1()
{
    m_Registers.set_de(pop());
    return 12;
}
int CPU::OP_0xE1()
{
    m_Registers.set_hl(pop());
    return 12;
}

//Word arithmatic
//ADD HL,n
int CPU::OP_0x09()
{
    word_t HL{ m_Registers.get_hl() };
    wordAdd(HL, m_Registers.get_bc());
    m_Registers.set_hl(HL);
    return 8;
}
int CPU::OP_0x19()
{
    word_t HL{ m_Registers.get_hl() };
    wordAdd(HL, m_Registers.get_de());
    m_Registers.set_hl(HL);
    return 8;
}
int CPU::OP_0x29()
{
    word_t HL{ m_Registers.get_hl() };
    wordAdd(HL, m_Registers.get_hl());
    m_Registers.set_hl(HL);
    return 8;
}
int CPU::OP_0x39()
{
    word_t HL{ m_Registers.get_hl() };
    wordAdd(HL, m_SP);
    m_Registers.set_hl(HL);
    return 8;
}
//ADD SP,n
int CPU::OP_0xE8()
{
    m_log(LOG_ERROR) << "Opcode 0xE8 needs to be signed" << "\n";
    std::exit(EXIT_FAILURE);
    wordAdd(m_SP, readNextWord());
    return 16;
}
//INC nn
int CPU::OP_0x03()
{
    m_Registers.set_bc(m_Registers.get_bc() + 1u);
    return 8;
}
int CPU::OP_0x13()
{
    m_Registers.set_de(m_Registers.get_de() + 1u);
    return 8;
}
int CPU::OP_0x23()
{
    m_Registers.set_hl(m_Registers.get_hl() + 1u);
    return 8;
}
int CPU::OP_0x33()
{
    ++m_SP;
    return 8;
}
//DEC nn
int CPU::OP_0x0B()
{
    m_Registers.set_bc(m_Registers.get_bc() - 1u);
    return 8;
}
int CPU::OP_0x1B()
{
    m_Registers.set_de(m_Registers.get_de() - 1u);
    return 8;
}
int CPU::OP_0x2B()
{
    m_Registers.set_hl(m_Registers.get_hl() - 1u);
    return 8;
}
int CPU::OP_0x3B()
{
    --m_SP;
    return 8;
}

//JP nn
int CPU::OP_0xC3()
{
    jump(JumpTest::Always, readNextWord());
    return 12;
}
//JP cc,nn
int CPU::OP_0xC2()
{
    jump(JumpTest::NotZero, readNextWord());
    return 12;
}
int CPU::OP_0xCA()
{
    jump(JumpTest::Zero, readNextWord());
    return 12;
}
int CPU::OP_0xD2()
{
    jump(JumpTest::NotCarry, readNextWord());
    return 12;
}
int CPU::OP_0xDA()
{
    jump(JumpTest::Carry, readNextWord());
    return 12;
}
//JP (HL)
int CPU::OP_0xE9()
{
    jump(JumpTest::Always, m_Registers.get_hl());
    return 4;
}
//Calls
//Call nn
int CPU::OP_0xCD()
{
    call(JumpTest::Always, readNextWord());
    return 12;
}
//CALL cc,nn
int CPU::OP_0xC4()
{
    call(JumpTest::NotZero, readNextWord());
    return 12;
}
int CPU::OP_0xCC()
{
    call(JumpTest::Zero, readNextWord());
    return 12;
}
int CPU::OP_0xD4()
{
    call(JumpTest::NotCarry, readNextWord());
    return 12;
}
int CPU::OP_0xDC()
{
    call(JumpTest::Carry, readNextWord());
    return 12;
}
//RET
int CPU::OP_0xC9()
{
    return_(JumpTest::Always);
    return 8;
}
//RET cc
int CPU::OP_0xC0()
{
    return_(JumpTest::NotZero);
    return 8;
}
int CPU::OP_0xC8()
{
    return_(JumpTest::Zero);
    return 8;
}
int CPU::OP_0xD0()
{
    return_(JumpTest::NotCarry);
    return 8;
}
int CPU::OP_0xD8()
{
    return_(JumpTest::Carry);
    return 8;
}
//RETI
int CPU::OP_0xD9()
{
    return_(JumpTest::Always);
    m_InteruptsEnabled = true;
    return 8;
}
//Miscellaneous
//DAA
int CPU::OP_0x27()
{
    checkDAA(m_Registers.a);
    return 4;
}
//CPL
int CPU::OP_0x2F()
{
    m_Registers.a = ~m_Registers.a;
    m_Registers.f.subtract = true;
    m_Registers.f.half_carry = true;
    return 4;
}
//CCF
int CPU::OP_0x3F()
{
    m_Registers.f.carry = !m_Registers.f.carry;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = false;
    return 4;
}
//SCF
int CPU::OP_0x37()
{
    m_Registers.f.carry = true;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = false;
    return 4;
}
//NOP
int CPU::OP_0x00()
{
    m_log(LOG_DEBUG) << "NOP" << "\n";
    return 4;
}
/*********************vvvvvv************************/
//HALT
int CPU::OP_0x76()
{
    m_log(LOG_INFO) << "Executed Halt!" << "\n";
    m_Halted = true;
    return 4;
}
//STOP
int CPU::OP_0x10()
{
    m_log(LOG_ERROR) << "Stop not implemented" << "\n";
    std::exit(EXIT_FAILURE);
    m_log(LOG_INFO) << "Executed Stop!" << "\n";
    return 4;
}
/***********************^^^^^^^************************/
//DI disable interupts
int CPU::OP_0xF3()
{
    m_InteruptsEnabled = false;
    m_log(LOG_INFO) << "Interupts Disabled!" << "\n";
    return 4;
}
//EI enable interupts
int CPU::OP_0xFB()
{
    m_InteruptsEnabled = true;
    m_log(LOG_INFO) << "Interupts Enabled!" << "\n";
    return 4;
}
//Rotates and shifts
//RLCA
int CPU::OP_0x07()
{
    leftRotate(m_Registers.a, 0);
    return 4;
}
//RLA
int CPU::OP_0x17()
{
    leftRotateWithCarry(m_Registers.a, 0);
    return 4;
}
//RRCA
int CPU::OP_0x0F()
{
    rightRotate(m_Registers.a, 0);
    return 4;
}
//RRA
int CPU::OP_0x1F()
{
    rightRotateWithCarry(m_Registers.a, 0);
    return 4;
}