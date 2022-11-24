#include "cpu.h"

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
//SWAP n
int CPU::OP_CB_0x37()
{
    swapNibbles(m_Registers.a);
    return 8;
}
int CPU::OP_CB_0x30()
{
    swapNibbles(m_Registers.b);
    return 8;
}
int CPU::OP_CB_0x31()
{
    swapNibbles(m_Registers.c);
    return 8;
}
int CPU::OP_CB_0x32()
{
    swapNibbles(m_Registers.d);
    return 8;
}
int CPU::OP_CB_0x33()
{
    swapNibbles(m_Registers.e);
    return 8;
}
int CPU::OP_CB_0x34()
{
    swapNibbles(m_Registers.h);
    return 8;
}
int CPU::OP_CB_0x35()
{
    swapNibbles(m_Registers.l);
    return 8;
}
int CPU::OP_CB_0x36()
{
    byte_t val{ m_Memory.readByte( m_Registers.get_hl()) };
    swapNibbles(val);
    m_Memory.writeByte(m_Registers.get_hl(), val);
    return 16;
}