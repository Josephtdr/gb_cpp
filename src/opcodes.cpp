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
    registers.set_hl( registers.get_hl()-1u );
    return 8;
}
//LD (HLD),A
//LD (HL-),A
//LDD (HL),A
int CPU::OP_0x32()
{
    byteLoad(ByteLoadTarget::HLI, ByteLoadSource::A);
    registers.set_hl( registers.get_hl()-1u );
    return 8;
}
//LD A,(HLI)
//LD A,(HL+)
//LDI A,(HL)
int CPU::OP_0x2A()
{
    byteLoad(ByteLoadTarget::A, ByteLoadSource::HLI);
    registers.set_hl( registers.get_hl()+1u );
    return 8;
}
//LD (HLI),A
//LD (HL+),A
//LDI (HL),A
int CPU::OP_0x22()
{
    byteLoad(ByteLoadTarget::HLI, ByteLoadSource::A);
    registers.set_hl( registers.get_hl()+1u );
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

    registers.f.zero = false;
    registers.f.subtract = false;

    registers.f.carry;
    registers.f.half_carry;

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
    push(registers.get_af());
    return 16;
}
int CPU::OP_0xC5()
{
    push(registers.get_bc());
    return 16;
}
int CPU::OP_0xD5()
{
    push(registers.get_de());
    return 16;
}
int CPU::OP_0xE5()
{
    push(registers.get_hl());
    return 16;
}
//POP nn
int CPU::OP_0xF1()
{
    registers.set_af(pop());
    return 12;
}
int CPU::OP_0xC1()
{
    registers.set_bc(pop());
    return 12;
}
int CPU::OP_0xD1()
{
    registers.set_de(pop());
    return 12;
}
int CPU::OP_0xE1()
{
    registers.set_hl(pop());
    return 12;
}
//ADD A,n
int CPU::OP_0x87()
{
    byteAdd(ByteAluSource::A);
    return 4;
}
int CPU::OP_0x80()
{
    byteAdd(ByteAluSource::B);
    return 4;
}
int CPU::OP_0x81()
{
    byteAdd(ByteAluSource::C);
    return 4;
}
int CPU::OP_0x82()
{
    byteAdd(ByteAluSource::D);
    return 4;
}
int CPU::OP_0x83()
{
    byteAdd(ByteAluSource::E);
    return 4;
}
int CPU::OP_0x84()
{
    byteAdd(ByteAluSource::H);
    return 4;
}
int CPU::OP_0x85()
{
    byteAdd(ByteAluSource::L);
    return 4;
}
int CPU::OP_0x86()
{
    byteAdd(ByteAluSource::HLI);
    return 8;
}
int CPU::OP_0xC6()
{
    byteAdd(ByteAluSource::D8);
    return 8;
}
//ADC A,n
int CPU::OP_0x8F()
{
    byteAdd(ByteAluSource::A, true);
    return 4;
}
int CPU::OP_0x88()
{
    byteAdd(ByteAluSource::B, true);
    return 4;
}
int CPU::OP_0x89()
{
    byteAdd(ByteAluSource::C, true);
    return 4;
}
int CPU::OP_0x8A()
{
    byteAdd(ByteAluSource::D, true);
    return 4;
}
int CPU::OP_0x8B()
{
    byteAdd(ByteAluSource::E, true);
    return 4;
}
int CPU::OP_0x8C()
{
    byteAdd(ByteAluSource::H, true);
    return 4;
}
int CPU::OP_0x8D()
{
    byteAdd(ByteAluSource::L, true);
    return 4;
}
int CPU::OP_0x8E()
{
    byteAdd(ByteAluSource::HLI, true);
    return 8;
}
int CPU::OP_0xCE()
{
    byteAdd(ByteAluSource::D8, true);
    return 8;
}
//SUB A, n
int CPU::OP_0x97()
{
    byteSub(ByteAluSource::A);
    return 4;
}
int CPU::OP_0x90()
{
    byteSub(ByteAluSource::B);
    return 4;
}
int CPU::OP_0x91()
{
    byteSub(ByteAluSource::C);
    return 4;
}
int CPU::OP_0x92()
{
    byteSub(ByteAluSource::D);
    return 4;
}
int CPU::OP_0x93()
{
    byteSub(ByteAluSource::E);
    return 4;
}
int CPU::OP_0x94()
{
    byteSub(ByteAluSource::H);
    return 4;
}
int CPU::OP_0x95()
{
    byteSub(ByteAluSource::L);
    return 4;
}
int CPU::OP_0x96()
{
    byteSub(ByteAluSource::HLI);
    return 8;
}
int CPU::OP_0xD6()
{
    byteSub(ByteAluSource::D8);
    return 8;
}
//SBC A,n
int CPU::OP_0x9F()
{
    byteSub(ByteAluSource::A, true);
    return 4;
}
int CPU::OP_0x98()
{
    byteSub(ByteAluSource::B, true);
    return 4;
}
int CPU::OP_0x99()
{
    byteSub(ByteAluSource::C, true);
    return 4;
}
int CPU::OP_0x9A()
{
    byteSub(ByteAluSource::D, true);
    return 4;
}
int CPU::OP_0x9B()
{
    byteSub(ByteAluSource::E, true);
    return 4;
}
int CPU::OP_0x9C()
{
    byteSub(ByteAluSource::H, true);
    return 4;
}
int CPU::OP_0x9D()
{
    byteSub(ByteAluSource::L, true);
    return 4;
}
int CPU::OP_0x9E()
{
    byteSub(ByteAluSource::HLI, true);
    return 8;
}
//AND A, n
int CPU::OP_0xA7()
{
    byteAND(ByteAluSource::A);
    return 4;
}
int CPU::OP_0xA0()
{
    byteAND(ByteAluSource::B);
    return 4;
}
int CPU::OP_0xA1()
{
    byteAND(ByteAluSource::C);
    return 4;
}
int CPU::OP_0xA2()
{
    byteAND(ByteAluSource::D);
    return 4;
}
int CPU::OP_0xA3()
{
    byteAND(ByteAluSource::E);
    return 4;
}
int CPU::OP_0xA4()
{
    byteAND(ByteAluSource::H);
    return 4;
}
int CPU::OP_0xA5()
{
    byteAND(ByteAluSource::L);
    return 4;
}
int CPU::OP_0xA6()
{
    byteAND(ByteAluSource::HLI);
    return 8;
}
int CPU::OP_0xE6()
{
    byteAND(ByteAluSource::D8);
    return 8;
}
//OR A, n
int CPU::OP_0xB7()
{
    byteOR(registers.a,registers.a);
    return 4;
}
int CPU::OP_0xB0()
{
    byteOR(registers.a,registers.b);
    return 4;
}
int CPU::OP_0xB1()
{
    byteOR(registers.a,registers.c);
    return 4;
}
int CPU::OP_0xB2()
{
    byteOR(registers.a,registers.d);
    return 4;
}
int CPU::OP_0xB3()
{
    byteOR(registers.a,registers.e);
    return 4;
}
int CPU::OP_0xB4()
{
    byteOR(registers.a,registers.h);
    return 4;
}
int CPU::OP_0xB5()
{
    byteOR(registers.a,registers.l);
    return 4;
}
int CPU::OP_0xB6()
{
    byteOR(registers.a,memory.readByte(registers.get_hl()));
    return 8;
}
int CPU::OP_0xF6()
{
    byteOR(registers.a,0,true);
    return 8;
}
//XOR A, n
int CPU::OP_0xAF()
{
    byteXOR(registers.a,registers.a);
    return 4;
}
int CPU::OP_0xA8()
{
    byteXOR(registers.a,registers.b);
    return 4;
}
int CPU::OP_0xA9()
{
    byteXOR(registers.a,registers.c);
    return 4;
}
int CPU::OP_0xAA()
{
    byteXOR(registers.a,registers.d);
    return 4;
}
int CPU::OP_0xAB()
{
    byteXOR(registers.a,registers.e);
    return 4;
}
int CPU::OP_0xAC()
{
    byteXOR(registers.a,registers.h);
    return 4;
}
int CPU::OP_0xAD()
{
    byteXOR(registers.a,registers.l);
    return 4;
}
int CPU::OP_0xAE()
{
    byteXOR(registers.a,memory.readByte(registers.get_hl()));
    return 8;
}
int CPU::OP_0xEE()
{
    byteXOR(registers.a,0,true);
    return 8;
}
//CP A, n
int CPU::OP_0xBF()
{
    byteCP(registers.a, registers.a);
    return 4;
}
int CPU::OP_0xB8()
{
    byteCP(registers.a, registers.b);
    return 4;
}
int CPU::OP_0xB9()
{
    byteCP(registers.a, registers.c);
    return 4;
}
int CPU::OP_0xBA()
{
    byteCP(registers.a, registers.d);
    return 4;
}
int CPU::OP_0xBB()
{
    byteCP(registers.a, registers.e);
    return 4;
}
int CPU::OP_0xBC()
{
    byteCP(registers.a, registers.h);
    return 4;
}
int CPU::OP_0xBD()
{
    byteCP(registers.a, registers.l);
    return 4;
}
int CPU::OP_0xBE()
{
    byteCP(registers.a, memory.readByte(registers.get_hl()));
    return 8;
}
int CPU::OP_0xFE()
{
    byteCP(registers.a, 0, true);
    return 8;
}
//INC n
int CPU::OP_0x3C()
{
    byteINC(registers.a);
    return 4;
}
int CPU::OP_0x04()
{
    byteINC(registers.b);
    return 4;
}
int CPU::OP_0x0C()
{
    byteINC(registers.c);
    return 4;
}
int CPU::OP_0x14()
{
    byteINC(registers.d);
    return 4;
}
int CPU::OP_0x1C()
{
    byteINC(registers.e);
    return 4;
}
int CPU::OP_0x24()
{
    byteINC(registers.h);
    return 4;
}
int CPU::OP_0x2C()
{
    byteINC(registers.l);
    return 4;
}
int CPU::OP_0x34()
{
    byte_t hli{ memory.readByte(registers.get_hl()) };
    byteINC(hli);
    memory.writeByte(registers.get_hl(), hli);
    return 12;
}
//DEC n
int CPU::OP_0x3D()
{
    byteDEC(registers.a);
    return 4;
}
int CPU::OP_0x05()
{
    byteDEC(registers.b);
    return 4;
}
int CPU::OP_0x0D()
{
    byteDEC(registers.c);
    return 4;
}
int CPU::OP_0x15()
{
    byteDEC(registers.d);
    return 4;
}
int CPU::OP_0x1D()
{
    byteDEC(registers.e);
    return 4;
}
int CPU::OP_0x25()
{
    byteDEC(registers.h);
    return 4;
}
int CPU::OP_0x2D()
{
    byteDEC(registers.l);
    return 4;
}
int CPU::OP_0x35()
{
    byte_t hli{ memory.readByte(registers.get_hl()) };
    byteDEC(hli);
    memory.writeByte(registers.get_hl(), hli);
    return 12;
}



///////////////
int CPU::OP_OxF1()
{
    registers.set_af( pop() );
    return 12;
} //POP AF
int CPU::OP_OxC1()
{
    registers.set_bc( pop() );
    return 12;
} //POP BC
int CPU::OP_OxD1()
{
    registers.set_de( pop() );
    return 12;
} //POP DE
int CPU::OP_OxE1()
{
    registers.set_hl( pop() );
    return 12;
} //POP HL
