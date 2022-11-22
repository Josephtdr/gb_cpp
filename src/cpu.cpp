#include "cpu.h"

#include <stdexcept> // for std::runtime_error

CPU::CPU()
 : pc{ c_INITIAL_PC_VALUE }, sp{ c_TOP_OF_STACK }
{
}

int CPU::execute(byte_t instructionByte, bool prefixed)
{
    if (!prefixed)
        { return ((*this).*(instructionTable[instructionByte]))(); }
    else
        { return ((*this).*(prefixedInstructionTable[instructionByte]))(); }
}

int CPU::cycle()
{
    if (halted)
        return;

    byte_t instructionByte{ memory.readByte(pc) };

    bool prefixed{ instructionByte == c_PREFIXED_INSTRUCTION_BYTE };
    if (prefixed)
    { 
        ++pc;
        instructionByte = memory.readByte(pc); 
    }
    ++pc;
    int nCycles{ execute(instructionByte, prefixed) };

    return nCycles;
}

word_t CPU::readNextWord()
{
    byte_t lsb{ memory.readByte(pc++) };
    byte_t msb{ memory.readByte(pc++) };
    return (msb << 8) | lsb;
}
byte_t CPU::readNextByte()
{
    return memory.readByte(pc++);
}

void CPU::noOperation() {}
void CPU::halt()
{
    halted = true;
}

void CPU::push(word_t value)
{
    --sp;
    memory.writeByte(sp, ((value & 0xFF00u) >> 8));
    --sp;
    memory.writeByte(sp, (value & 0xFFu));
}
word_t CPU::pop()
{
    byte_t lsb{ memory.readByte(sp) };
    ++sp;
    byte_t msb{ memory.readByte(sp) };
    ++sp;
    return (msb << 8) | lsb;
}
word_t CPU::call(bool valid)
{
    word_t nextPC{ pc + 2u };
    if (valid)
    {
        push(nextPC);
        return readNextWord();
    }
    else
        { return nextPC; }
}
word_t CPU::return_(bool valid)
{
    if (valid)
        { return pop(); }
    else
        { return pc + 1u; }
}


word_t CPU::jump(JumpTest type)
{   
    bool valid{};
    switch (type)
    {
        case JumpTest::Zero:
            valid = registers.f.zero; break;
        case JumpTest::NotZero:
            valid = !registers.f.zero; break;
        case JumpTest::Carry:
            valid = registers.f.carry; break;
        case JumpTest::NotCarry:
            valid = !registers.f.carry; break;
        case JumpTest::Always:
            valid = true; break;
        default:
            throw std::runtime_error("Invalid jump type!");
    }
    // Gameboy is little endian so read pc + 2 as most significant bit
    // and pc + 1 as least significant bit
    if (valid)
    {
        byte_t msb{ memory.readByte(pc+1u) };
        byte_t lsb{ memory.readByte(pc) };
        
        return (msb << 8) | lsb;
    }
    else //skip the jump
    { 
        return (pc + 2u); 
    }
}

void CPU::byteLoad(ByteLoadTarget ldTarget, ByteLoadSource ldSource)
{
    byte_t value{};
    switch (ldSource)
    {
        case ByteLoadSource::A:
            value = registers.a; break;
        case ByteLoadSource::B:
            value = registers.b; break;
        case ByteLoadSource::C:
            value = registers.c; break;
        case ByteLoadSource::D:
            value = registers.d; break;
        case ByteLoadSource::E:
            value = registers.e; break;
        case ByteLoadSource::H:
            value = registers.h; break;
        case ByteLoadSource::L:
            value = registers.l; break;
        case ByteLoadSource::D8:
            value = readNextByte(); break;
        case ByteLoadSource::BCI:
            value = memory.readByte(registers.get_bc()); break;
        case ByteLoadSource::DEI:
            value = memory.readByte(registers.get_de()); break;
        case ByteLoadSource::HLI:
            value = memory.readByte(registers.get_hl()); break;
        case ByteLoadSource::D16I:
            value = memory.readByte(readNextWord()); break;
        case ByteLoadSource::FF00pC:
            value = memory.readByte(0xFF00u+registers.c); break;
        case ByteLoadSource::FF00pD8:
            value = memory.readByte(0xFF00u+readNextByte()); break;
        default:
            throw std::runtime_error("Invalid load source!");
    }
    //D16I, BCI, DEI, HLI,
    switch (ldTarget)
    {
        case ByteLoadTarget::A:
            registers.a = value; break;
        case ByteLoadTarget::B:
            registers.b = value; break;
        case ByteLoadTarget::C:
            registers.c = value; break;
        case ByteLoadTarget::D:
            registers.d = value; break;
        case ByteLoadTarget::E:
            registers.e = value; break;
        case ByteLoadTarget::H:
            registers.h = value; break;
        case ByteLoadTarget::L:
            registers.l = value; break;
        case ByteLoadTarget::BCI:
            memory.writeByte(registers.get_bc(), value); break;
        case ByteLoadTarget::DEI:
            memory.writeByte(registers.get_de(), value); break;
        case ByteLoadTarget::HLI:
            memory.writeByte(registers.get_hl(), value); break;
        case ByteLoadTarget::D16I:
            memory.writeByte(readNextWord(), value); break;
        case ByteLoadTarget::FF00pC:
            memory.writeByte(0xFF00u+registers.c, value); break;
        case ByteLoadTarget::FF00pD8:
            memory.writeByte(0xFF00u+readNextByte(), value); break;
        default:
            throw std::runtime_error("Invalid load source!");
    }
}