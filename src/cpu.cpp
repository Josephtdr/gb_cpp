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
        return 1; //not sure what return value should be here, check interupts

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
/**
 * @brief Reads the next word from memory according to the pc, 
 * lsb first, also increments the pc twice.
 * @return byte_t 
 */
word_t CPU::readNextWord()
{
    byte_t lsb{ memory.readByte(pc++) };
    byte_t msb{ memory.readByte(pc++) };
    return (msb << 8) | lsb;
}
/**
 * @brief Reads the next byte from memory according to the pc, 
 * also increments the pc.
 * @return byte_t 
 */
byte_t CPU::readNextByte()
{
    return memory.readByte(pc++);
}

void CPU::noOperation() {}
void CPU::halt()
{
    halted = true;
}

/**
 * @brief Adds the given value to the stack, a byte at a time, msb first, 
 * also decrements the sp twice.
 * @param value the value to be added to the stack
 */
void CPU::push(word_t value)
{
    --sp;
    memory.writeByte(sp, ((value & 0xFF00u) >> 8));
    --sp;
    memory.writeByte(sp, (value & 0xFFu));
}
/**
 * @brief Pops a value of the top of the stack 
 * and increments the sp twice
 * @return word_t the value from the top of the stack
 */
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
    word_t nextPC{ static_cast<word_t>(pc + 2u) };
    if (valid)
    {
        push(nextPC);
        return readNextWord();
    }
    else
        { return nextPC; }
}
/**
 * @brief Checks if the return is valid, if so 
 * pops the value from the top of the stack and returns it
 * otherwise returns the current pc such that the return is ignored.
 * 
 * @param valid boolean indicating if the return is valid
 * @return the address to return to
 */
word_t CPU::return_(bool valid)
{
    if (valid)
        { return pop(); }
    else
        { return pc; }
}

/**
 * @brief 
 * Check if the pc should jump, if so returns the word stores in the next two bytes of memory, otherwise skips those bytes
 * @param type The type of test to use, to determine wether to jump or not
 * @return the address to jump to
 */
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
    // Gameboy is little endian hance pc + 1 is most significant byte
    // and pc is least significant byte
    if (valid)
    {
        byte_t msb{ memory.readByte(pc+1u) };
        byte_t lsb{ memory.readByte(pc) };
        
        return (msb << 8) | lsb;
    }
    else //skip the jump address
    { 
        return (pc + 2u); 
    }
}
/**
 * @brief Loads a byte value from the source into the target.
 * HCI etc. means the address location of the value stored in HC,
 * D8 and D16 mean the next byte or word read from memory
 * A lowercase p indicates plus, FF00pC means 0xFF00 + the value in register C.
 * @param ldTarget where the value is loaded into.
 * @param ldSource where the value is taken from.
 */
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
            throw std::runtime_error("Invalid load target!");
    }
}
/**
 * @brief Loads a word value from the source into the target.
 * D16I etc. means the address location of the value from D16,
 * D8 and D16 mean the next byte or word read from memory
 * A lowercase p indicates plus, SPpD8 means the stack pointer + D8.
 * @param ldTarget where the value is loaded into.
 * @param ldSource where the value is taken from.
 */
void CPU::wordLoad(WordLoadTarget ldTarget, WordLoadSource ldSource)
{
    word_t value{};
    switch(ldSource)
    {   
        case WordLoadSource::HL:
            value = registers.get_hl(); break;
        case WordLoadSource::SP:
            value = sp; break;
        case WordLoadSource::D16:
            value = readNextWord(); break;
        case WordLoadSource::SPpD8:
            {
                byte_t d8{ readNextByte() };
                registers.f.zero = false;
                registers.f.subtract = false;

                unsigned int v{ static_cast<unsigned int>(sp)+static_cast<unsigned int>(d8) };
                bool carry{ v > 0xFFFF };
                registers.f.carry = carry;

                bool half_carry{ ((sp & 0xF) + (d8 & 0xF)) > 0xF };
                registers.f.half_carry = half_carry;

                value = sp+d8; 
                break;
            }
        default:
            throw std::runtime_error("Invalid load source!");
    } 

    switch(ldTarget)
    {
        case WordLoadTarget::BC:
            registers.set_bc(value); break;
        case WordLoadTarget::DE:
            registers.set_de(value); break;
        case WordLoadTarget::HL:
            registers.set_hl(value); break;
        case WordLoadTarget::SP:
            sp = value; break;
        case WordLoadTarget::D16I:
            memory.writeByte(readNextWord(), value); break;
        default:
            throw std::runtime_error("Invalid load target!");
    }
}
/**
 * @brief Add a byte to register A, 
 * Zero flag set if result is 0, subtract flag is reset, 
 * Carry flag set if carry from bit 7, Half Carry flag set if carry from bit 3. 
 * @param addSource the byte to add
 */
void CPU::byteAdd(ByteAddSource addSource, bool withCarry=false)
{
    word_t value{};
    switch (addSource)
    {
    case ByteAddSource::A:
        value = registers.a; break;
    case ByteAddSource::B:
        value = registers.b; break;
    case ByteAddSource::C:
        value = registers.c; break;
    case ByteAddSource::D:
        value = registers.d; break;
    case ByteAddSource::E:
        value = registers.e; break;
    case ByteAddSource::H:
        value = registers.h; break;
    case ByteAddSource::L:
        value = registers.l; break;
    case ByteAddSource::HLI:
        value = memory.readByte(registers.get_hl()); break;
    case ByteAddSource::D8:
        value = readNextByte(); break;
    default:
        throw std::runtime_error("Invalid add source!");
    }

    if (withCarry)
        { ++value; }

    unsigned int v{ static_cast<unsigned int>(registers.a)+static_cast<unsigned int>(value) };
    bool carry{ v > 0xFF };
    registers.f.carry = carry;

    bool half_carry{ ((registers.a & 0xF) + (value & 0xF)) > 0xF };
    registers.f.half_carry = half_carry;

    registers.a += value;

    bool zero{ !registers.a };
    registers.f.zero = zero;

    registers.f.subtract = false;
}