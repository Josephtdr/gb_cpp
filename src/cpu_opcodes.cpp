#include "inc/cpu.h"

#include <stdexcept>
#include <iostream>

void CPU::setupTables()
{
    for(byte_t i = 0; i < c_INSTRUCTION_TABLE_SIZE; ++i)
    {
        instructionTable[i] = &CPU::OP_NOT_IMPLEMTED;
    }

    for (byte_t i=0x06; i<=0x3E; i+=8u)
    {
        instructionTable[i] = &CPU::cpu_byteLoad;
    }
    for (byte_t i=0x40; i<=0x7F; ++i)
    {
        instructionTable[i] = &CPU::cpu_byteLoad;
    }
    for (byte_t i=0x80; i<=0xBF; ++i)
    {
        instructionTable[i] = &CPU::cpu_byteArithmetic;
    }
    for (byte_t i=0xC6; i>=0xC6; i+=8)
    {
        instructionTable[i] = &CPU::cpu_byteArithmetic;
    }
    for (byte_t i=0x05; i<=0x3D; i+=8)
    {
        instructionTable[i] = &CPU::cpu_byteIncDec;
    }
    for (byte_t i=0x04; i<=0x3C; i+=8)
    {
        instructionTable[i] = &CPU::cpu_byteIncDec;
    }
    for (byte_t i=0x03; i<=0x3B; i+=8)
    {
        instructionTable[i] = &CPU::cpu_WordIncDec;
    }
    for (byte_t i=0xC7; i>=0xC7; i+=8)
    {
        instructionTable[i] = &CPU::cpu_restart;
    }
    for (byte_t i=0xC0; i<=0xD8; i+=8)
    {   
        for (byte_t j=0; j<=2; ++j)
        {
            instructionTable[i + 2u*j] = &CPU::cpu_jump;
        }
    }
    instructionTable[0xC9] = &CPU::cpu_jump;
    instructionTable[0xC3] = &CPU::cpu_jump;
    instructionTable[0xCD] = &CPU::cpu_jump;

    for (byte_t i=0xC1; i>=0xC1; i+=0x10u)
    {   
        instructionTable[i] = &CPU::cpu_wordPopPush;
        instructionTable[i + 0x4u] = &CPU::cpu_wordPopPush;
    }
    for (byte_t i=0x18; i<=0x38; i+=8)
    {
        instructionTable[i] = &CPU::cpu_jumpRelative;
    }
    for (byte_t i=0x02; i<=0x3A; i+=8)
    {
        instructionTable[i] = &CPU::cpu_byteIndexLoad;
    }
    for (byte_t i=0x09; i<=0x39; i+=0x10)
    {
        instructionTable[i] = &CPU::cpu_wordAdd;
    }
    for (byte_t i=0x01; i<=0x31; i+=0x10)
    {
        instructionTable[i] = &CPU::cpu_wordLoad;
    }
    
    //unique commands
    instructionTable[0x76] = &CPU::cpu_halt;
    instructionTable[0x10] = &CPU::cpu_stop;
    instructionTable[0x27] = &CPU::cpu_checkDAA;
    instructionTable[0xE9] = &CPU::cpu_jumpHL;
    instructionTable[0xD9] = &CPU::cpu_RETI;
    instructionTable[0x2F] = &CPU::cpu_CPL;
    instructionTable[0x3F] = &CPU::cpu_CCF;
    instructionTable[0x37] = &CPU::cpu_SCF;
    instructionTable[0x00] = &CPU::cpu_NOP;
    instructionTable[0xF3] = &CPU::cpu_DI;
    instructionTable[0xFB] = &CPU::cpu_EI;
    instructionTable[0x07] = &CPU::cpu_RLCA;
    instructionTable[0x17] = &CPU::cpu_RLA;
    instructionTable[0x0F] = &CPU::cpu_RRCA;
    instructionTable[0x1F] = &CPU::cpu_RRA;
    instructionTable[0xE8] = &CPU::cpu_ADDSPr8;
    instructionTable[0x08] = &CPU::cpu_LDd8SP;
    instructionTable[0xFA] = &CPU::cpu_LDAd16;
    instructionTable[0xEA] = &CPU::cpu_LDd16A;
    instructionTable[0xF2] = &CPU::cpu_0xF2; //LD A,(C)
    instructionTable[0xE2] = &CPU::cpu_0xE2; //LD (C),A
    instructionTable[0xE0] = &CPU::cpu_0xE0; //LDH (n),A
    instructionTable[0xF0] = &CPU::cpu_0xF0; //LDH A,(n)
    instructionTable[0xF8] = &CPU::cpu_0xF8; //LD (nn),SP signed
    
    //Prefix Instruction
    for (byte_t i = 0; i < 0x8; ++i)
    {
        preCB0x40_FunctionTable[i] = &CPU::leftRotate;
    }
    for (byte_t i = 0x8; i < 0x10; ++i)
    {
        preCB0x40_FunctionTable[i] = &CPU::rightRotate;
    }
    for (byte_t i = 0x10; i < 0x18; ++i)
    {
        preCB0x40_FunctionTable[i] = &CPU::leftRotateWithCarry;
    }
    for (byte_t i = 0x18; i < 0x20; ++i)
    {
        preCB0x40_FunctionTable[i] = &CPU::rightRotateWithCarry;
    }
    for (byte_t i = 0x20; i < 0x28; ++i)
    {
        preCB0x40_FunctionTable[i] = &CPU::leftShift;
    }
    for (byte_t i = 0x28; i < 0x30; ++i)
    {
        preCB0x40_FunctionTable[i] = &CPU::rightShiftArithmetic;
    }
    for (byte_t i = 0x30; i < 0x38; ++i)
    {
        preCB0x40_FunctionTable[i] = &CPU::swapNibbles;
    }
    for (byte_t i = 0x38; i < 0x40; ++i)
    {
        preCB0x40_FunctionTable[i] = &CPU::rightShift;
    }
    for (byte_t i = 0x40; i < 0x80; ++i)
    {
        postCB0x40_FunctionTable[i-0x40] = &CPU::testBit_OP;
    }
    for (byte_t i = 0x80; i < 0xC0; ++i)
    {
        postCB0x40_FunctionTable[i-0x40] = &CPU::resetBit;
    }
    for (byte_t i = 0xC0; i >= 0xC0; ++i)
    {
        postCB0x40_FunctionTable[i-0x40] = &CPU::setBit;
    }

    std::cout << "Tables Initialised!\n";
}

int CPU::OP_NOT_IMPLEMTED(const byte_t& opcode)
{
    throw std::runtime_error("OPCODE NOT IMPLEMENTED!");
}

int CPU::cpu_CB_opcode_Translator(byte_t opcode)
{
    unsigned int regInt{ opcode & 0xFu }; 
    bool usingHLI{ regInt == 6 || regInt == 14 };
    

    if (opcode < 0x40)
    {
        if (!usingHLI)
        {
            byte_t& reg = getRegister(opcode%8);
            ((*this).*(preCB0x40_FunctionTable[opcode]))(reg);
            return 8;
        }
        else
        {
            byte_t HL{ m_Memory.readByte(m_Registers.get_hl()) };
            ((*this).*(preCB0x40_FunctionTable[opcode]))(HL);
            m_Memory.writeByte(m_Registers.get_hl(), HL);
            return 16;
        }
    }
    else
    {
        int bit{ (opcode % 0x40) / 8 };
        // m_log(LOG_DEBUG) << "CB Opcode: " << +opcode << ", performing on bit: " << bit << "\n";  
        if (!usingHLI)
        {
            byte_t& reg = getRegister(opcode%8);
            // m_log(LOG_INFO) << "reg before is: " << +reg << ", f: "<< +m_Registers.f <<".\n";
            ((*this).*(postCB0x40_FunctionTable[opcode-0x40]))(reg, bit);
            // m_log(LOG_INFO) << "reg after is: " << +reg << ", f: "<< +m_Registers.f <<".\n";
            return 8;
        }
        else
        {
            byte_t HL{ m_Memory.readByte(m_Registers.get_hl()) };
            ((*this).*(postCB0x40_FunctionTable[opcode-0x40]))(HL, bit);
            m_Memory.writeByte(m_Registers.get_hl(), HL);
            return opcode < 0x80 ? 12 : 16;
        }
    }
}



int CPU::cpu_LDAd16(const byte_t&)
{
    m_Registers.a = m_Memory.readByte(readNextWord());
    return 16;
}
int CPU::cpu_LDd16A(const byte_t&)
{
    m_Memory.writeByte(readNextWord(), m_Registers.a);
    return  16;
}
//LD A,(C)
int CPU::cpu_0xF2(const byte_t&)
{
    m_Registers.a = m_Memory.readByte(0xFF00u + m_Registers.c);
    return 8;
}
//LD (C),A
int CPU::cpu_0xE2(const byte_t&)
{
    m_Memory.writeByte(0xFF00u + m_Registers.c, m_Registers.a);
    return 8;
}
//LDH (n),A
int CPU::cpu_0xE0(const byte_t&)
{
    m_Memory.writeByte(0xFF00u + readNextByte(), m_Registers.a);
    return 12;
}
//LDH A,(n)
int CPU::cpu_0xF0(const byte_t&)
{
    m_Registers.a = m_Memory.readByte(0xFF00u + readNextByte());
    return 12;
}
//LD HL,SP+n
//LDHL SP,n
int CPU::cpu_0xF8(const byte_t&)
{
    m_log(LOG_ERROR) << "opcode 0xF8 needs to be signed!" << "\n";
    std::exit(EXIT_FAILURE);

    word_t unsignedData{ readNextByte() };
    word_t HL{ m_Registers.get_hl() };


    // unsigned int v{ static_cast<unsigned int>(m_SP)+static_cast<unsigned int>(d8) };
    // bool carry{ v > 0xFFFF };
    // m_Registers.f.carry = carry;

    // bool half_carry{ ((m_SP & 0xF) + (d8 & 0xF)) > 0xF };
    // m_Registers.f.half_carry = half_carry;

    
    m_Registers.set_hl( HL + m_SP + unsignedData );

    m_Registers.f.zero = false;
    m_Registers.f.subtract = false;

    return 12;
}

//Miscellaneous
int CPU::cpu_LDd8SP(const byte_t&)
{
    word_t address{ readNextWord() };
    m_Memory.writeByte(address, m_SP);
    return 20;
}
//ADD SP,n
int CPU::cpu_ADDSPr8(const byte_t&)
{
    m_log(LOG_ERROR) << "Opcode 0xE8 needs to be signed" << "\n";
    std::exit(EXIT_FAILURE);

    word_t unsignedData{ readNextWord() };

    m_SP += unsignedData;

    return 16;
}
//CPL
int CPU::cpu_CPL(const byte_t&)
{
    m_Registers.a = ~m_Registers.a;
    m_Registers.f.subtract = true;
    m_Registers.f.half_carry = true;
    return 4;
}
//CCF
int CPU::cpu_CCF(const byte_t&)
{
    m_Registers.f.carry = !m_Registers.f.carry;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = false;
    return 4;
}
//SCF
int CPU::cpu_SCF(const byte_t&)
{
    m_Registers.f.carry = true;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = false;
    return 4;
}
//NOP
int CPU::cpu_NOP(const byte_t&)
{
    m_log(LOG_DEBUG) << "NOP" << "\n";
    return 4;
}
int CPU::cpu_RETI(const byte_t&)
{
    return_(4);
    m_InteruptsEnabled = true;
    return 8;
}

int CPU::cpu_jumpHL(const byte_t&)
{
    m_PC = m_Registers.get_hl();   
    return 4;
}
/*********************vvvvvv************************/
//HALT
int CPU::cpu_halt(const byte_t&)
{
    m_log(LOG_ERROR) << "Halt not implemented" << "\n";
    std::exit(EXIT_FAILURE);
    m_log(LOG_INFO) << "Executed Halt!" << "\n";
    return 4;
}
//STOP
int CPU::cpu_stop(const byte_t&)
{
    m_log(LOG_ERROR) << "Stop not implemented" << "\n";
    std::exit(EXIT_FAILURE);
    m_log(LOG_INFO) << "Executed Stop!" << "\n";
    return 4;
}
/***********************^^^^^^^************************/
//DI disable interupts
int CPU::cpu_DI(const byte_t&)
{
    m_InteruptsEnabled = false;
    m_log(LOG_INFO) << "Interupts Disabled!" << "\n";
    return 4;
}
//EI enable interupts
int CPU::cpu_EI(const byte_t&)
{
    m_InteruptsEnabled = true;
    m_log(LOG_INFO) << "Interupts Enabled!" << "\n";
    return 4;
}
//Rotates and shifts
//RLCA
int CPU::cpu_RLCA(const byte_t&)
{
    leftRotate(m_Registers.a);
    return 4;
}
//RLA
int CPU::cpu_RLA(const byte_t&)
{
    leftRotateWithCarry(m_Registers.a);
    return 4;
}
//RRCA
int CPU::cpu_RRCA(const byte_t&)
{
    rightRotate(m_Registers.a);
    return 4;
}
//RRA
int CPU::cpu_RRA(const byte_t&)
{
    rightRotateWithCarry(m_Registers.a);
    return 4;
}