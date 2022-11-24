#include "cpu.h"

#include <stdexcept>

void CPU::setupTables()
{
    for(byte_t i = 0; i < c_INSTRUCTION_TABLE_SIZE; ++i)
    {
        instructionTable[i] = &CPU::OP_NOT_IMPLEMTED;
    }

    //LD nn,n
    instructionTable[0x06] = &CPU::OP_0x06;
    instructionTable[0x0E] = &CPU::OP_0x0E;
    instructionTable[0x16] = &CPU::OP_0x16;
    instructionTable[0x1E] = &CPU::OP_0x1E;
    instructionTable[0x26] = &CPU::OP_0x26;
    instructionTable[0x2E] = &CPU::OP_0x2E;
    //LD r1, r2
    instructionTable[0x7F] = &CPU::OP_0x7F;
    instructionTable[0x78] = &CPU::OP_0x78;
    instructionTable[0x79] = &CPU::OP_0x79;
    instructionTable[0x7A] = &CPU::OP_0x7A;
    instructionTable[0x7B] = &CPU::OP_0x7B;
    instructionTable[0x7C] = &CPU::OP_0x7C;
    instructionTable[0x7D] = &CPU::OP_0x7D;
    instructionTable[0x7E] = &CPU::OP_0x7E;
    instructionTable[0x40] = &CPU::OP_0x40;
    instructionTable[0x41] = &CPU::OP_0x41;
    instructionTable[0x42] = &CPU::OP_0x42;
    instructionTable[0x43] = &CPU::OP_0x43;
    instructionTable[0x44] = &CPU::OP_0x44;
    instructionTable[0x45] = &CPU::OP_0x45;
    instructionTable[0x46] = &CPU::OP_0x46;
    instructionTable[0x48] = &CPU::OP_0x48;
    instructionTable[0x49] = &CPU::OP_0x49;
    instructionTable[0x4A] = &CPU::OP_0x4A;
    instructionTable[0x4B] = &CPU::OP_0x4B;
    instructionTable[0x4C] = &CPU::OP_0x4C;
    instructionTable[0x4D] = &CPU::OP_0x4D;
    instructionTable[0x4E] = &CPU::OP_0x4E;
    instructionTable[0x50] = &CPU::OP_0x50;
    instructionTable[0x51] = &CPU::OP_0x51;
    instructionTable[0x52] = &CPU::OP_0x52;
    instructionTable[0x53] = &CPU::OP_0x53;
    instructionTable[0x54] = &CPU::OP_0x54;
    instructionTable[0x55] = &CPU::OP_0x55;
    instructionTable[0x56] = &CPU::OP_0x56;
    instructionTable[0x58] = &CPU::OP_0x58;
    instructionTable[0x59] = &CPU::OP_0x59;
    instructionTable[0x5A] = &CPU::OP_0x5A;
    instructionTable[0x5B] = &CPU::OP_0x5B;
    instructionTable[0x5C] = &CPU::OP_0x5C;
    instructionTable[0x5D] = &CPU::OP_0x5D;
    instructionTable[0x5E] = &CPU::OP_0x5E;
    instructionTable[0x60] = &CPU::OP_0x60;
    instructionTable[0x61] = &CPU::OP_0x61;
    instructionTable[0x62] = &CPU::OP_0x62;
    instructionTable[0x63] = &CPU::OP_0x63;
    instructionTable[0x64] = &CPU::OP_0x64;
    instructionTable[0x65] = &CPU::OP_0x65;
    instructionTable[0x66] = &CPU::OP_0x66;
    instructionTable[0x68] = &CPU::OP_0x68;
    instructionTable[0x69] = &CPU::OP_0x69;
    instructionTable[0x6A] = &CPU::OP_0x6A;
    instructionTable[0x6B] = &CPU::OP_0x6B;
    instructionTable[0x6C] = &CPU::OP_0x6C;
    instructionTable[0x6D] = &CPU::OP_0x6D;
    instructionTable[0x6E] = &CPU::OP_0x6E;
    instructionTable[0x70] = &CPU::OP_0x70;
    instructionTable[0x71] = &CPU::OP_0x71;
    instructionTable[0x72] = &CPU::OP_0x72;
    instructionTable[0x73] = &CPU::OP_0x73;
    instructionTable[0x74] = &CPU::OP_0x74;
    instructionTable[0x75] = &CPU::OP_0x75;
    instructionTable[0x36] = &CPU::OP_0x36;
    //LD A,n
    instructionTable[0x0A] = &CPU::OP_0x0A;
    instructionTable[0x1A] = &CPU::OP_0x1A;
    instructionTable[0xFA] = &CPU::OP_0xFA;
    instructionTable[0x3E] = &CPU::OP_0x3E;
    //LD n,A
    instructionTable[0x47] = &CPU::OP_0x47;
    instructionTable[0x4F] = &CPU::OP_0x4F;
    instructionTable[0x57] = &CPU::OP_0x57;
    instructionTable[0x5F] = &CPU::OP_0x5F;
    instructionTable[0x67] = &CPU::OP_0x67;
    instructionTable[0x6F] = &CPU::OP_0x6F;
    instructionTable[0x02] = &CPU::OP_0x02;
    instructionTable[0x12] = &CPU::OP_0x12;
    instructionTable[0x77] = &CPU::OP_0x77;
    instructionTable[0xEA] = &CPU::OP_0xEA;
    //LD A,(C)
    instructionTable[0xF2] = &CPU::OP_0xF2;
    //LD (C),A
    instructionTable[0xE2] = &CPU::OP_0xE2;
    //LD A,(HLD)
    //LD A,(HL-)
    //LDD A,(HL)
    instructionTable[0x3A] = &CPU::OP_0x3A;
    //LD (HLD),A
    //LD (HL-),A
    //LDD (HL),A
    instructionTable[0x32] = &CPU::OP_0x32;
    //LD A,(HLI)
    //LD A,(HL+)
    //LDI A,(HL)
    instructionTable[0x2A] = &CPU::OP_0x2A;
    //LD (HLI),A
    //LD (HL+),A
    //LDI (HL),A
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
    //ADD A,n
    instructionTable[0x87] = &CPU::OP_0x87;
    instructionTable[0x80] = &CPU::OP_0x80;
    instructionTable[0x81] = &CPU::OP_0x81;
    instructionTable[0x82] = &CPU::OP_0x82;
    instructionTable[0x83] = &CPU::OP_0x83;
    instructionTable[0x84] = &CPU::OP_0x84;
    instructionTable[0x85] = &CPU::OP_0x85;
    instructionTable[0x86] = &CPU::OP_0x86;
    instructionTable[0xC6] = &CPU::OP_0xC6;
    //ADC A,n
    instructionTable[0x8F] = &CPU::OP_0x8F;
    instructionTable[0x88] = &CPU::OP_0x88;
    instructionTable[0x89] = &CPU::OP_0x89;
    instructionTable[0x8A] = &CPU::OP_0x8A;
    instructionTable[0x8B] = &CPU::OP_0x8B;
    instructionTable[0x8C] = &CPU::OP_0x8C;
    instructionTable[0x8D] = &CPU::OP_0x8D;
    instructionTable[0x8E] = &CPU::OP_0x8E;
    instructionTable[0xCE] = &CPU::OP_0xCE;
    //SUB A, n
    instructionTable[0x97] = &CPU::OP_0x97;
    instructionTable[0x90] = &CPU::OP_0x90;
    instructionTable[0x91] = &CPU::OP_0x91;
    instructionTable[0x92] = &CPU::OP_0x92;
    instructionTable[0x93] = &CPU::OP_0x93;
    instructionTable[0x94] = &CPU::OP_0x94;
    instructionTable[0x95] = &CPU::OP_0x95;
    instructionTable[0x96] = &CPU::OP_0x96;
    instructionTable[0xD6] = &CPU::OP_0xD6;
    //SBC A,n
    instructionTable[0x9F] = &CPU::OP_0x9F;
    instructionTable[0x98] = &CPU::OP_0x98;
    instructionTable[0x99] = &CPU::OP_0x99;
    instructionTable[0x9A] = &CPU::OP_0x9A;
    instructionTable[0x9B] = &CPU::OP_0x9B;
    instructionTable[0x9C] = &CPU::OP_0x9C;
    instructionTable[0x9D] = &CPU::OP_0x9D;
    instructionTable[0x9E] = &CPU::OP_0x9E;
    // int OP_0x??(); rip D8
    //AND A, n
    instructionTable[0xA7] = &CPU::OP_0xA7;
    instructionTable[0xA0] = &CPU::OP_0xA0;
    instructionTable[0xA1] = &CPU::OP_0xA1;
    instructionTable[0xA2] = &CPU::OP_0xA2;
    instructionTable[0xA3] = &CPU::OP_0xA3;
    instructionTable[0xA4] = &CPU::OP_0xA4;
    instructionTable[0xA5] = &CPU::OP_0xA5;
    instructionTable[0xA6] = &CPU::OP_0xA6;
    instructionTable[0xE6] = &CPU::OP_0xE6;
    //OR A, n
    instructionTable[0xB7] = &CPU::OP_0xB7;
    instructionTable[0xB0] = &CPU::OP_0xB0;
    instructionTable[0xB1] = &CPU::OP_0xB1;
    instructionTable[0xB2] = &CPU::OP_0xB2;
    instructionTable[0xB3] = &CPU::OP_0xB3;
    instructionTable[0xB4] = &CPU::OP_0xB4;
    instructionTable[0xB5] = &CPU::OP_0xB5;
    instructionTable[0xB6] = &CPU::OP_0xB6;
    instructionTable[0xF6] = &CPU::OP_0xF6;
    //XOR A, n
    instructionTable[0xAF] = &CPU::OP_0xAF;
    instructionTable[0xA8] = &CPU::OP_0xA8;
    instructionTable[0xA9] = &CPU::OP_0xA9;
    instructionTable[0xAA] = &CPU::OP_0xAA;
    instructionTable[0xAB] = &CPU::OP_0xAB;
    instructionTable[0xAC] = &CPU::OP_0xAC;
    instructionTable[0xAD] = &CPU::OP_0xAD;
    instructionTable[0xAE] = &CPU::OP_0xAE;
    instructionTable[0xEE] = &CPU::OP_0xEE;
    //CP A, n
    instructionTable[0xBF] = &CPU::OP_0xBF;
    instructionTable[0xB8] = &CPU::OP_0xB8;
    instructionTable[0xB9] = &CPU::OP_0xB9;
    instructionTable[0xBA] = &CPU::OP_0xBA;
    instructionTable[0xBB] = &CPU::OP_0xBB;
    instructionTable[0xBC] = &CPU::OP_0xBC;
    instructionTable[0xBD] = &CPU::OP_0xBD;
    instructionTable[0xBE] = &CPU::OP_0xBE;
    instructionTable[0xFE] = &CPU::OP_0xFE;
    //INC n
    instructionTable[0x3C] = &CPU::OP_0x3C;
    instructionTable[0x04] = &CPU::OP_0x04;
    instructionTable[0x0C] = &CPU::OP_0x0C;
    instructionTable[0x14] = &CPU::OP_0x14;
    instructionTable[0x1C] = &CPU::OP_0x1C;
    instructionTable[0x24] = &CPU::OP_0x24;
    instructionTable[0x2C] = &CPU::OP_0x2C;
    instructionTable[0x34] = &CPU::OP_0x34;
    //DEC n
    instructionTable[0x3D] = &CPU::OP_0x3D;
    instructionTable[0x05] = &CPU::OP_0x05;
    instructionTable[0x0D] = &CPU::OP_0x0D;
    instructionTable[0x15] = &CPU::OP_0x15;
    instructionTable[0x1D] = &CPU::OP_0x1D;
    instructionTable[0x25] = &CPU::OP_0x25;
    instructionTable[0x2D] = &CPU::OP_0x2D;
    instructionTable[0x35] = &CPU::OP_0x35;
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
    instructionTable[0x18] = &CPU::OP_0x18;
    //JR cc,n
    instructionTable[0x20] = &CPU::OP_0x20;
    instructionTable[0x28] = &CPU::OP_0x28;
    instructionTable[0x30] = &CPU::OP_0x30;
    instructionTable[0x38] = &CPU::OP_0x38;
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
    instructionTable[0xC7] = &CPU::OP_0xC7;
    instructionTable[0xCF] = &CPU::OP_0xCF;
    instructionTable[0xD7] = &CPU::OP_0xD7;
    instructionTable[0xDF] = &CPU::OP_0xDF;
    instructionTable[0xE7] = &CPU::OP_0xE7;
    instructionTable[0xEF] = &CPU::OP_0xEF;
    instructionTable[0xF7] = &CPU::OP_0xF7;
    instructionTable[0xFF] = &CPU::OP_0xFF;
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
}

int CPU::OP_NOT_IMPLEMTED()
{
    throw std::runtime_error("OPCODE NOT IMPLEMENTED!");
}

//Prefixed instructions
byte_t& CPU::CBopcodeToRegister(byte_t opcode)
{
    switch(opcode & 0xFu)
    {
        case 0: case 8:
        return m_Registers.b;
        case 1: case 9:
        return m_Registers.c; break;
        case 2: case 10:
        return m_Registers.d; break;
        case 3: case 11:
        return m_Registers.e; break;
        case 4: case 12:
        return m_Registers.h; break;
        case 5: case 13:
        return m_Registers.l; break;
        case 7: case 15:
        return m_Registers.a; break;
        default: 
            throw std::runtime_error("INVALID OPCODE used in CPU::CBopcodeToRegister");
    }
}

int CPU::CBopcode_Translator(byte_t opcode)
{
    unsigned int regInt{ opcode & 0xFu }; 
    bool usingHLI{ regInt != 6 and regInt != 14 };

    if (opcode < 0x40)
    {
        if (!usingHLI)
        {
            byte_t& reg = CBopcodeToRegister(opcode);
            ((*this).*(preCB0x40_FunctionTable[opcode-0x40]))(reg);
            return 8;
        }
        else
        {
            byte_t HL{ m_Memory.readByte(m_Registers.get_hl()) };
            ((*this).*(preCB0x40_FunctionTable[opcode-0x40]))(HL);
            m_Memory.writeByte(m_Registers.get_hl(), HL);
            return opcode < 0x80 ? 12 : 16;
        }
    }
    else
    {
        int bit{ (opcode % 0x40) / 8 };
        if (!usingHLI)
        {
            byte_t& reg = CBopcodeToRegister(opcode);
            ((*this).*(postCB0x40_FunctionTable[opcode-0x40]))(reg, bit);
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

//Non prefixed Instructions
//byte Loads
//LD nn,n
int CPU::OP_0x06()
{
    byteLoad(ByteLoadTarget::B, ByteLoadSource::D8);
    return 8;
}
int CPU::OP_0x0E()
{
    byteLoad(ByteLoadTarget::C, ByteLoadSource::D8);
    return 8;
}
int CPU::OP_0x16()
{
    byteLoad(ByteLoadTarget::D, ByteLoadSource::D8);
    return 8;
}
int CPU::OP_0x1E()
{
    byteLoad(ByteLoadTarget::E, ByteLoadSource::D8);
    return 8;
}
int CPU::OP_0x26()
{
    byteLoad(ByteLoadTarget::H, ByteLoadSource::D8);
    return 8;
}
int CPU::OP_0x2E()
{
    byteLoad(ByteLoadTarget::L, ByteLoadSource::D8);
    return 8;
}
//LD r1, r2
int CPU::OP_0x7F()
{
    byteLoad(ByteLoadTarget::A, ByteLoadSource::A);
    return 4;
}
int CPU::OP_0x78()
{
    byteLoad(ByteLoadTarget::A, ByteLoadSource::B);
    return 4;
}
int CPU::OP_0x79()
{
    byteLoad(ByteLoadTarget::A, ByteLoadSource::C);
    return 4;
}
int CPU::OP_0x7A()
{
    byteLoad(ByteLoadTarget::A, ByteLoadSource::D);
    return 4;
}
int CPU::OP_0x7B()
{
    byteLoad(ByteLoadTarget::A, ByteLoadSource::E);
    return 4;
}
int CPU::OP_0x7C()
{
    byteLoad(ByteLoadTarget::A, ByteLoadSource::H);
    return 4;
}
int CPU::OP_0x7D()
{
    byteLoad(ByteLoadTarget::A, ByteLoadSource::L);
    return 4;
}
int CPU::OP_0x7E()
{
    byteLoad(ByteLoadTarget::A, ByteLoadSource::HLI);
    return 8;
}
int CPU::OP_0x40()
{
    byteLoad(ByteLoadTarget::B, ByteLoadSource::B);
    return 4;
}
int CPU::OP_0x41()
{
    byteLoad(ByteLoadTarget::B, ByteLoadSource::C);
    return 4;
}
int CPU::OP_0x42()
{
    byteLoad(ByteLoadTarget::B, ByteLoadSource::D);
    return 4;
}
int CPU::OP_0x43()
{
    byteLoad(ByteLoadTarget::B, ByteLoadSource::E);
    return 4;
}
int CPU::OP_0x44()
{
    byteLoad(ByteLoadTarget::B, ByteLoadSource::H);
    return 4;
}
int CPU::OP_0x45()
{
    byteLoad(ByteLoadTarget::B, ByteLoadSource::L);
    return 4;
}
int CPU::OP_0x46()
{
    byteLoad(ByteLoadTarget::B, ByteLoadSource::HLI);
    return 8;
}
int CPU::OP_0x48()
{
    byteLoad(ByteLoadTarget::C, ByteLoadSource::B);
    return 4;
}
int CPU::OP_0x49()
{
    byteLoad(ByteLoadTarget::C, ByteLoadSource::C);
    return 4;
}
int CPU::OP_0x4A()
{
    byteLoad(ByteLoadTarget::C, ByteLoadSource::D);
    return 4;
}
int CPU::OP_0x4B()
{
    byteLoad(ByteLoadTarget::C, ByteLoadSource::E);
    return 4;
}
int CPU::OP_0x4C()
{
    byteLoad(ByteLoadTarget::C, ByteLoadSource::H);
    return 4;
}
int CPU::OP_0x4D()
{
    byteLoad(ByteLoadTarget::C, ByteLoadSource::L);
    return 4;
}
int CPU::OP_0x4E()
{
    byteLoad(ByteLoadTarget::C, ByteLoadSource::HLI);
    return 8;
}
int CPU::OP_0x50()
{
    byteLoad(ByteLoadTarget::D, ByteLoadSource::B);
    return 4;
}
int CPU::OP_0x51()
{
    byteLoad(ByteLoadTarget::D, ByteLoadSource::C);
    return 4;
}
int CPU::OP_0x52()
{
    byteLoad(ByteLoadTarget::D, ByteLoadSource::D);
    return 4;
}
int CPU::OP_0x53()
{
    byteLoad(ByteLoadTarget::D, ByteLoadSource::E);
    return 4;
}
int CPU::OP_0x54()
{
    byteLoad(ByteLoadTarget::D, ByteLoadSource::H);
    return 4;
}
int CPU::OP_0x55()
{
    byteLoad(ByteLoadTarget::D, ByteLoadSource::L);
    return 4;
}
int CPU::OP_0x56()
{
    byteLoad(ByteLoadTarget::D, ByteLoadSource::HLI);
    return 8;
}
int CPU::OP_0x58()
{
    byteLoad(ByteLoadTarget::E, ByteLoadSource::B);
    return 4;
}
int CPU::OP_0x59()
{
    byteLoad(ByteLoadTarget::E, ByteLoadSource::C);
    return 4;
}
int CPU::OP_0x5A()
{
    byteLoad(ByteLoadTarget::E, ByteLoadSource::D);
    return 4;
}
int CPU::OP_0x5B()
{
    byteLoad(ByteLoadTarget::E, ByteLoadSource::E);
    return 4;
}
int CPU::OP_0x5C()
{
    byteLoad(ByteLoadTarget::E, ByteLoadSource::H);
    return 4;
}
int CPU::OP_0x5D()
{
    byteLoad(ByteLoadTarget::E, ByteLoadSource::L);
    return 4;
}
int CPU::OP_0x5E()
{
    byteLoad(ByteLoadTarget::E, ByteLoadSource::HLI);
    return 8;
}
int CPU::OP_0x60()
{
    byteLoad(ByteLoadTarget::H, ByteLoadSource::B);
    return 4;
}
int CPU::OP_0x61()
{
    byteLoad(ByteLoadTarget::H, ByteLoadSource::C);
    return 4;
}
int CPU::OP_0x62()
{
    byteLoad(ByteLoadTarget::H, ByteLoadSource::D);
    return 4;
}
int CPU::OP_0x63()
{
    byteLoad(ByteLoadTarget::H, ByteLoadSource::E);
    return 4;
}
int CPU::OP_0x64()
{
    byteLoad(ByteLoadTarget::H, ByteLoadSource::H);
    return 4;
}
int CPU::OP_0x65()
{
    byteLoad(ByteLoadTarget::H, ByteLoadSource::L);
    return 4;
}
int CPU::OP_0x66()
{
    byteLoad(ByteLoadTarget::H, ByteLoadSource::HLI);
    return 8;
}
int CPU::OP_0x68()
{
    byteLoad(ByteLoadTarget::L, ByteLoadSource::B);
    return 4;
}
int CPU::OP_0x69()
{
    byteLoad(ByteLoadTarget::L, ByteLoadSource::C);
    return 4;
}
int CPU::OP_0x6A()
{
    byteLoad(ByteLoadTarget::L, ByteLoadSource::D);
    return 4;
}
int CPU::OP_0x6B()
{
    byteLoad(ByteLoadTarget::L, ByteLoadSource::E);
    return 4;
}
int CPU::OP_0x6C()
{
    byteLoad(ByteLoadTarget::L, ByteLoadSource::H);
    return 4;
}
int CPU::OP_0x6D()
{
    byteLoad(ByteLoadTarget::L, ByteLoadSource::L);
    return 4;
}
int CPU::OP_0x6E()
{
    byteLoad(ByteLoadTarget::L, ByteLoadSource::HLI);
    return 8;
}
int CPU::OP_0x70()
{
    byteLoad(ByteLoadTarget::HLI, ByteLoadSource::B);
    return 8;
}
int CPU::OP_0x71()
{
    byteLoad(ByteLoadTarget::HLI, ByteLoadSource::C);
    return 8;
}
int CPU::OP_0x72()
{
    byteLoad(ByteLoadTarget::HLI, ByteLoadSource::D);
    return 8;
}
int CPU::OP_0x73()
{
    byteLoad(ByteLoadTarget::HLI, ByteLoadSource::E);
    return 8;
}
int CPU::OP_0x74()
{
    byteLoad(ByteLoadTarget::HLI, ByteLoadSource::H);
    return 8;
}
int CPU::OP_0x75()
{
    byteLoad(ByteLoadTarget::HLI, ByteLoadSource::L);
    return 8;
}
int CPU::OP_0x36()
{
    byteLoad(ByteLoadTarget::HLI, ByteLoadSource::D8);
    return 12;
}
//LD A,n
int CPU::OP_0x0A()
{
    byteLoad(ByteLoadTarget::A, ByteLoadSource::BCI);
    return 8;
}
int CPU::OP_0x1A()
{
    byteLoad(ByteLoadTarget::A, ByteLoadSource::DEI);
    return 8;
}
int CPU::OP_0xFA()
{
    byteLoad(ByteLoadTarget::A, ByteLoadSource::D16I);
    return 16;
}
int CPU::OP_0x3E()
{
    byteLoad(ByteLoadTarget::A, ByteLoadSource::D8);
    return 8;
}
//LD n,A
int CPU::OP_0x47()
{
    byteLoad(ByteLoadTarget::B, ByteLoadSource::A);
    return  4;
}
int CPU::OP_0x4F()
{
    byteLoad(ByteLoadTarget::C, ByteLoadSource::A);
    return  4;
}
int CPU::OP_0x57()
{
    byteLoad(ByteLoadTarget::D, ByteLoadSource::A);
    return  4;
}
int CPU::OP_0x5F()
{
    byteLoad(ByteLoadTarget::E, ByteLoadSource::A);
    return  4;
}
int CPU::OP_0x67()
{
    byteLoad(ByteLoadTarget::H, ByteLoadSource::A);
    return  4;
}
int CPU::OP_0x6F()
{
    byteLoad(ByteLoadTarget::L, ByteLoadSource::A);
    return  4;
}
int CPU::OP_0x02()
{
    byteLoad(ByteLoadTarget::BCI, ByteLoadSource::A);
    return  8;
}
int CPU::OP_0x12()
{
    byteLoad(ByteLoadTarget::DEI, ByteLoadSource::A);
    return  8;
}
int CPU::OP_0x77()
{
    byteLoad(ByteLoadTarget::HLI, ByteLoadSource::A);
    return  8;
}
int CPU::OP_0xEA()
{
    byteLoad(ByteLoadTarget::D16I, ByteLoadSource::A);
    return  16;
}
//LD A,(C)
int CPU::OP_0xF2()
{
    byteLoad(ByteLoadTarget::A, ByteLoadSource::FF00pC);
    return 8;
}
//LD (C),A
int CPU::OP_0xE2()
{
    byteLoad(ByteLoadTarget::FF00pC, ByteLoadSource::A);
    return 8;
}
//LD A,(HLD)
//LD A,(HL-)
//LDD A,(HL)
int CPU::OP_0x3A()
{
    byteLoad(ByteLoadTarget::A, ByteLoadSource::HLI);
    m_Registers.set_hl( m_Registers.get_hl()-1u );
    return 8;
}
//LD (HLD),A
//LD (HL-),A
//LDD (HL),A
int CPU::OP_0x32()
{
    byteLoad(ByteLoadTarget::HLI, ByteLoadSource::A);
    m_Registers.set_hl( m_Registers.get_hl()-1u );
    return 8;
}
//LD A,(HLI)
//LD A,(HL+)
//LDI A,(HL)
int CPU::OP_0x2A()
{
    byteLoad(ByteLoadTarget::A, ByteLoadSource::HLI);
    m_Registers.set_hl( m_Registers.get_hl()+1u );
    return 8;
}
//LD (HLI),A
//LD (HL+),A
//LDI (HL),A
int CPU::OP_0x22()
{
    byteLoad(ByteLoadTarget::HLI, ByteLoadSource::A);
    m_Registers.set_hl( m_Registers.get_hl()+1u );
    return 8;
}
//LDH (n),A
int CPU::OP_0xE0()
{
    byteLoad(ByteLoadTarget::FF00pD8, ByteLoadSource::A);
    return 12;
}
//LDH A,(n)
int CPU::OP_0xF0()
{
    byteLoad(ByteLoadTarget::A, ByteLoadSource::FF00pD8);
    return 12;
}

//*********************************************************//

//WordLoads
//LD n,nn
int CPU::OP_0x01()
{
    wordLoad(WordLoadTarget::BC, WordLoadSource::D16);
    return 12;
}
int CPU::OP_0x11()
{
    wordLoad(WordLoadTarget::DE, WordLoadSource::D16);
    return 12;
}
int CPU::OP_0x21()
{
    wordLoad(WordLoadTarget::HL, WordLoadSource::D16);
    return 12;
}
int CPU::OP_0x31()
{
    wordLoad(WordLoadTarget::SP, WordLoadSource::D16);
    return 12;
}
//LD SP,HL
int CPU::OP_0xF9()
{
    wordLoad(WordLoadTarget::SP, WordLoadSource::HL);
    return 8;
}
//LD HL,SP+n
//LDHL SP,n
int CPU::OP_0xF8()
{
    wordLoad(WordLoadTarget::HL, WordLoadSource::SPpD8);
    return 12;
}
//LD (nn),SP
int CPU::OP_0x08()
{
    wordLoad(WordLoadTarget::D16I, WordLoadSource::SP);
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
    m_Registers.set_af(pop());
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
//ADD A,n
int CPU::OP_0x87()
{
    byteAdd(m_Registers.a,m_Registers.a);
    return 4;
}
int CPU::OP_0x80()
{
    byteAdd(m_Registers.a,m_Registers.b);
    return 4;
}
int CPU::OP_0x81()
{
    byteAdd(m_Registers.a,m_Registers.c);
    return 4;
}
int CPU::OP_0x82()
{
    byteAdd(m_Registers.a,m_Registers.d);
    return 4;
}
int CPU::OP_0x83()
{
    byteAdd(m_Registers.a,m_Registers.e);
    return 4;
}
int CPU::OP_0x84()
{
    byteAdd(m_Registers.a,m_Registers.h);
    return 4;
}
int CPU::OP_0x85()
{
    byteAdd(m_Registers.a,m_Registers.l);
    return 4;
}
int CPU::OP_0x86()
{
    byteAdd(m_Registers.a, m_Memory.readByte(m_Registers.get_hl()));
    return 8;
}
int CPU::OP_0xC6()
{
    byteAdd(m_Registers.a, readNextByte());
    return 8;
}
//ADC A,n
int CPU::OP_0x8F()
{
    byteAdd(m_Registers.a,m_Registers.a, true);
    return 4;
}
int CPU::OP_0x88()
{
    byteAdd(m_Registers.a,m_Registers.b, true);
    return 4;
}
int CPU::OP_0x89()
{
    byteAdd(m_Registers.a,m_Registers.c, true);
    return 4;
}
int CPU::OP_0x8A()
{
    byteAdd(m_Registers.a,m_Registers.d, true);
    return 4;
}
int CPU::OP_0x8B()
{
    byteAdd(m_Registers.a,m_Registers.e, true);
    return 4;
}
int CPU::OP_0x8C()
{
    byteAdd(m_Registers.a,m_Registers.h, true);
    return 4;
}
int CPU::OP_0x8D()
{
    byteAdd(m_Registers.a,m_Registers.l, true);
    return 4;
}
int CPU::OP_0x8E()
{
    byteAdd(m_Registers.a, m_Memory.readByte(m_Registers.get_hl()), true);
    return 8;
}
int CPU::OP_0xCE()
{
    byteAdd(m_Registers.a, readNextByte(), true);
    return 8;
}
//SUB A, n
int CPU::OP_0x97()
{
    byteSub(m_Registers.a,m_Registers.a);
    return 4;
}
int CPU::OP_0x90()
{
    byteSub(m_Registers.a,m_Registers.b);
    return 4;
}
int CPU::OP_0x91()
{
    byteSub(m_Registers.a,m_Registers.c);
    return 4;
}
int CPU::OP_0x92()
{
    byteSub(m_Registers.a,m_Registers.d);
    return 4;
}
int CPU::OP_0x93()
{
    byteSub(m_Registers.a,m_Registers.e);
    return 4;
}
int CPU::OP_0x94()
{
    byteSub(m_Registers.a,m_Registers.h);
    return 4;
}
int CPU::OP_0x95()
{
    byteSub(m_Registers.a,m_Registers.l);
    return 4;
}
int CPU::OP_0x96()
{
    byteSub(m_Registers.a, m_Memory.readByte(m_Registers.get_hl()));
    return 8;
}
int CPU::OP_0xD6()
{
    byteSub(m_Registers.a, readNextByte());
    return 8;
}
//SBC A,n
int CPU::OP_0x9F()
{
    byteSub(m_Registers.a,m_Registers.a, true);
    return 4;
}
int CPU::OP_0x98()
{
    byteSub(m_Registers.a,m_Registers.b, true);
    return 4;
}
int CPU::OP_0x99()
{
    byteSub(m_Registers.a,m_Registers.c, true);
    return 4;
}
int CPU::OP_0x9A()
{
    byteSub(m_Registers.a,m_Registers.d, true);
    return 4;
}
int CPU::OP_0x9B()
{
    byteSub(m_Registers.a,m_Registers.e, true);
    return 4;
}
int CPU::OP_0x9C()
{
    byteSub(m_Registers.a,m_Registers.h, true);
    return 4;
}
int CPU::OP_0x9D()
{
    byteSub(m_Registers.a,m_Registers.l, true);
    return 4;
}
int CPU::OP_0x9E()
{
    byteSub(m_Registers.a, m_Memory.readByte(m_Registers.get_hl()), true);
    return 8;
}
//AND A, n
int CPU::OP_0xA7()
{
    byteAND(m_Registers.a,m_Registers.a);
    return 4;
}
int CPU::OP_0xA0()
{
    byteAND(m_Registers.a,m_Registers.b);
    return 4;
}
int CPU::OP_0xA1()
{
    byteAND(m_Registers.a,m_Registers.c);
    return 4;
}
int CPU::OP_0xA2()
{
    byteAND(m_Registers.a,m_Registers.d);
    return 4;
}
int CPU::OP_0xA3()
{
    byteAND(m_Registers.a,m_Registers.e);
    return 4;
}
int CPU::OP_0xA4()
{
    byteAND(m_Registers.a,m_Registers.h);
    return 4;
}
int CPU::OP_0xA5()
{
    byteAND(m_Registers.a,m_Registers.l);
    return 4;
}
int CPU::OP_0xA6()
{
    byteAND(m_Registers.a,m_Memory.readByte(m_Registers.get_hl()));
    return 8;
}
int CPU::OP_0xE6()
{
    byteAND(m_Registers.a,readNextByte());
    return 8;
}
//OR A, n
int CPU::OP_0xB7()
{
    byteOR(m_Registers.a,m_Registers.a);
    return 4;
}
int CPU::OP_0xB0()
{
    byteOR(m_Registers.a,m_Registers.b);
    return 4;
}
int CPU::OP_0xB1()
{
    byteOR(m_Registers.a,m_Registers.c);
    return 4;
}
int CPU::OP_0xB2()
{
    byteOR(m_Registers.a,m_Registers.d);
    return 4;
}
int CPU::OP_0xB3()
{
    byteOR(m_Registers.a,m_Registers.e);
    return 4;
}
int CPU::OP_0xB4()
{
    byteOR(m_Registers.a,m_Registers.h);
    return 4;
}
int CPU::OP_0xB5()
{
    byteOR(m_Registers.a,m_Registers.l);
    return 4;
}
int CPU::OP_0xB6()
{
    byteOR(m_Registers.a,m_Memory.readByte(m_Registers.get_hl()));
    return 8;
}
int CPU::OP_0xF6()
{
    byteOR(m_Registers.a,readNextByte());
    return 8;
}
//XOR A, n
int CPU::OP_0xAF()
{
    byteXOR(m_Registers.a,m_Registers.a);
    return 4;
}
int CPU::OP_0xA8()
{
    byteXOR(m_Registers.a,m_Registers.b);
    return 4;
}
int CPU::OP_0xA9()
{
    byteXOR(m_Registers.a,m_Registers.c);
    return 4;
}
int CPU::OP_0xAA()
{
    byteXOR(m_Registers.a,m_Registers.d);
    return 4;
}
int CPU::OP_0xAB()
{
    byteXOR(m_Registers.a,m_Registers.e);
    return 4;
}
int CPU::OP_0xAC()
{
    byteXOR(m_Registers.a,m_Registers.h);
    return 4;
}
int CPU::OP_0xAD()
{
    byteXOR(m_Registers.a,m_Registers.l);
    return 4;
}
int CPU::OP_0xAE()
{
    byteXOR(m_Registers.a,m_Memory.readByte(m_Registers.get_hl()));
    return 8;
}
int CPU::OP_0xEE()
{
    byteXOR(m_Registers.a,readNextByte());
    return 8;
}
//CP A, n
int CPU::OP_0xBF()
{
    byteCP(m_Registers.a, m_Registers.a);
    return 4;
}
int CPU::OP_0xB8()
{
    byteCP(m_Registers.a, m_Registers.b);
    return 4;
}
int CPU::OP_0xB9()
{
    byteCP(m_Registers.a, m_Registers.c);
    return 4;
}
int CPU::OP_0xBA()
{
    byteCP(m_Registers.a, m_Registers.d);
    return 4;
}
int CPU::OP_0xBB()
{
    byteCP(m_Registers.a, m_Registers.e);
    return 4;
}
int CPU::OP_0xBC()
{
    byteCP(m_Registers.a, m_Registers.h);
    return 4;
}
int CPU::OP_0xBD()
{
    byteCP(m_Registers.a, m_Registers.l);
    return 4;
}
int CPU::OP_0xBE()
{
    byteCP(m_Registers.a, m_Memory.readByte(m_Registers.get_hl()));
    return 8;
}
int CPU::OP_0xFE()
{
    byteCP(m_Registers.a,readNextByte());
    return 8;
}
//INC n
int CPU::OP_0x3C()
{
    byteINC(m_Registers.a);
    return 4;
}
int CPU::OP_0x04()
{
    byteINC(m_Registers.b);
    return 4;
}
int CPU::OP_0x0C()
{
    byteINC(m_Registers.c);
    return 4;
}
int CPU::OP_0x14()
{
    byteINC(m_Registers.d);
    return 4;
}
int CPU::OP_0x1C()
{
    byteINC(m_Registers.e);
    return 4;
}
int CPU::OP_0x24()
{
    byteINC(m_Registers.h);
    return 4;
}
int CPU::OP_0x2C()
{
    byteINC(m_Registers.l);
    return 4;
}
int CPU::OP_0x34()
{
    byte_t hli{ m_Memory.readByte(m_Registers.get_hl()) };
    byteINC(hli);
    m_Memory.writeByte(m_Registers.get_hl(), hli);
    return 12;
}
//DEC n
int CPU::OP_0x3D()
{
    byteDEC(m_Registers.a);
    return 4;
}
int CPU::OP_0x05()
{
    byteDEC(m_Registers.b);
    return 4;
}
int CPU::OP_0x0D()
{
    byteDEC(m_Registers.c);
    return 4;
}
int CPU::OP_0x15()
{
    byteDEC(m_Registers.d);
    return 4;
}
int CPU::OP_0x1D()
{
    byteDEC(m_Registers.e);
    return 4;
}
int CPU::OP_0x25()
{
    byteDEC(m_Registers.h);
    return 4;
}
int CPU::OP_0x2D()
{
    byteDEC(m_Registers.l);
    return 4;
}
int CPU::OP_0x35()
{
    byte_t hli{ m_Memory.readByte(m_Registers.get_hl()) };
    byteDEC(hli);
    m_Memory.writeByte(m_Registers.get_hl(), hli);
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
    wordAdd(m_SP, readNextWord());
    return 16;
}
//INC nn
int CPU::OP_0x03()
{
    m_Registers.set_bc(m_Registers.get_bc() + 1);
    return 8;
}
int CPU::OP_0x13()
{
    m_Registers.set_de(m_Registers.get_de() + 1);
    return 8;
}
int CPU::OP_0x23()
{
    m_Registers.set_hl(m_Registers.get_hl() + 1);
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
    m_Registers.set_bc(m_Registers.get_bc() - 1);
    return 8;
}
int CPU::OP_0x1B()
{
    m_Registers.set_de(m_Registers.get_de() - 1);
    return 8;
}
int CPU::OP_0x2B()
{
    m_Registers.set_hl(m_Registers.get_hl() - 1);
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
// JR n
int CPU::OP_0x18()
{
    jump(JumpTest::Always, m_PC+readNextByte());
    return 8;
}
//JR cc,n
int CPU::OP_0x20()
{
    jump(JumpTest::NotZero, m_PC+readNextByte());
    return 8;
}
int CPU::OP_0x28()
{
    jump(JumpTest::Zero,  m_PC+readNextByte());
    return 8;
}
int CPU::OP_0x30()
{
    jump(JumpTest::NotCarry,  m_PC+readNextByte());
    return 8;
}
int CPU::OP_0x38()
{
    jump(JumpTest::Carry,  m_PC+readNextByte());
    return 8;
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
//Restarts
//RST n
int CPU::OP_0xC7()
{
    restart(0x00);
    return  32;
}
int CPU::OP_0xCF()
{
    restart(0x08);
    return  32;
}
int CPU::OP_0xD7()
{
    restart(0x10);
    return  32;
}
int CPU::OP_0xDF()
{
    restart(0x18);
    return  32;
}
int CPU::OP_0xE7()
{
    restart(0x20);
    return  32;
}
int CPU::OP_0xEF()
{
    restart(0x28);
    return  32;
}
int CPU::OP_0xF7()
{
    restart(0x30);
    return  32;
}
int CPU::OP_0xFF()
{
    restart(0x38);
    return  32;
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
    return 4;
}
/*********************vvvvvv************************/
//HALT
int CPU::OP_0x76()
{

    return 4;
}
//STOP
int CPU::OP_0x10()
{

    return 4;
}
/***********************^^^^^^^************************/
//DI disable interupts
int CPU::OP_0xF3()
{
    m_InteruptsEnabled = false;
    return 4;
}
//EI enable interupts
int CPU::OP_0xFB()
{
    m_InteruptsEnabled = true;
    return 4;
}
//Rotates and shifts
//RLCA
int CPU::OP_0x07()
{
    leftRotate(m_Registers.a);
    return 4;
}
//RLA
int CPU::OP_0x17()
{
    leftRotateWithCarry(m_Registers.a);
    return 4;
}
//RRCA
int CPU::OP_0x0F()
{
    rightRotate(m_Registers.a);
    return 4;
}
//RRA
int CPU::OP_0x1F()
{
    rightRotateWithCarry(m_Registers.a);
    return 4;
}

// //Bit opcodes
// int CPU::testBit_Translator(byte_t opcode)
// {
//     byte_t start{ 0x40 };
//     int bit{ (opcode - start) / 8 };
//     unsigned int regInt{ opcode & 0xFu }; 

//     if (regInt != 6 and regInt != 14) // otherwise (HL)
//     {
//         byte_t& reg = CBopcodeToRegister(opcode);
//         testBit_OP(reg, bit);
//         return 8;
//     }
//     else
//     {
//         byte_t HL{ m_Memory.readByte(m_Registers.get_hl()) };
//         testBit_OP(HL, bit);
//         m_Memory.writeByte(m_Registers.get_hl(), HL);
//         return 16;
//     }
// }
// int CPU::resetBit_Translator(byte_t opcode)
// {
//     byte_t start{ 0x80 };
//     int bit{ (opcode - start) / 8 };
//     unsigned int regInt{ opcode & 0xFu }; 

//     if (regInt != 6 and regInt != 14) // otherwise (HL)
//     {
//         byte_t& reg = CBopcodeToRegister(opcode);
//         resetBit(reg, bit);
//         return 8;
//     }
//     else
//     {
//         byte_t HL{ m_Memory.readByte(m_Registers.get_hl()) };
//         resetBit(HL, bit);
//         m_Memory.writeByte(m_Registers.get_hl(), HL);
//         return 16;
//     }
// }
// int CPU::setBit_Translator(byte_t opcode)
// {
//     byte_t start{ 0xC0 };
//     int bit{ (opcode - start) / 8 };
//     unsigned int regInt{ opcode & 0xFu }; 

//     if (regInt != 6 and regInt != 14) // otherwise (HL)
//     {
//         byte_t& reg = CBopcodeToRegister(opcode);
//         setBit(reg, bit);
//         return 8;
//     }
//     else
//     {
//         byte_t HL{ m_Memory.readByte(m_Registers.get_hl()) };
//         setBit(HL, bit);
//         m_Memory.writeByte(m_Registers.get_hl(), HL);
//         return 16;
//     }
// }