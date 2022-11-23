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
/**
 * @brief 
 * 
 * @param type 
 * @param address 
 */
void CPU::call(JumpTest type, const word_t& address)
{
    if (testJumpTest(type))
    {
        push(pc);
        pc = address;
    }
}
/**
 * @brief Checks if the return is valid, if so 
 * pops the value from the top of the stack and returns it
 * otherwise returns the current pc such that the return is ignored.
 * 
 * @param valid boolean indicating if the return is valid
 * @return the address to return to
 */
void CPU::return_(JumpTest type)
{
    if (testJumpTest(type))
        { pc = pop(); }
}

void CPU::restart(byte_t address)
{
    push(pc);
    pc = (0x0000 + address);
}

bool CPU::testJumpTest(JumpTest type)
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
    return valid;
}

/**
 * @brief 
 * Check if the pc should jump, and if so, does
 * @param type The type of test to use, to determine wether to jump or not
 * @param address the address to jump to
 */
void CPU::jump(JumpTest type, const word_t& address)
{   
    if (testJumpTest(type))
        { pc = address; }
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
 * @brief add a byte to the given reg, 
 * Zero flag set if result is 0, subtract flag is reset, 
 * Carry flag set if carry from bit 7, Half Carry flag set if carry from bit 3.
 * @param reg the reg being altered
 * @param addValue the value to add with
 * @param withCarry include the carry flag in the subtraction
 */
void CPU::byteAdd(byte_t& reg, const byte_t& addValue, bool withCarry=false)
{
    byte_t value{ addValue };
    if (withCarry)
        { value += static_cast<byte_t>(registers.f.carry); }

    unsigned int ctest{ static_cast<unsigned int>(reg)+static_cast<unsigned int>(value) };
    word_t htest{ (reg & 0xFu) + (value & 0xFu) };
    
    reg += value;
    
    registers.f.zero = !reg;
    registers.f.subtract = false;
    registers.f.carry = ctest > 0xFFu;
    registers.f.half_carry = htest > 0xFu;
}
    
/**
 * @brief subtract a byte from the given reg, 
 * Zero flag set if result is 0, subtract flag is set, 
 * Carry flag set if no borrow, Half Carry flag set if borrow from bit 4.
 * @param reg the reg being altered
 * @param subValue the value to subtract with
 * @param withCarry include the carry flag in the subtraction
 */
void CPU::byteSub(byte_t& reg, const byte_t& subValue, bool withCarry=false)
{
    byte_t unchanged{ reg };
    byte_t value{ subValue };
    if (withCarry)
        { value += static_cast<byte_t>(registers.f.carry); }

    signed int htest{ reg & 0xF };
    htest -= static_cast<signed int>(value & 0xF);
    reg -= value;
    
    registers.f.zero = !reg;
    registers.f.subtract = true;
    registers.f.carry = unchanged < value ;
    registers.f.half_carry = htest < 0;
}


/**
 * @brief ands the passed reg with the given andValue
 * 
 * @param reg the reg being altered
 * @param andValue the value to and with
 */
void CPU::byteAND(byte_t& reg, const byte_t& andValue)
{
    reg &= andValue;

    registers.f.carry = false;
    registers.f.half_carry = true;
    registers.f.subtract = false;
    registers.f.zero = !reg;
}

/**
 * @brief ors the passed reg with the given orValue
 * 
 * @param reg the reg being altered
 * @param orValue the value to or with
 */
void CPU::byteOR(byte_t& reg, const byte_t& orValue)
{
    reg |= orValue;

    registers.f.carry = false;
    registers.f.half_carry = false;
    registers.f.subtract = false;
    registers.f.zero = !reg;
}

/**
 * @brief xors the passed reg with the given xorValue
 * 
 * @param reg the reg being altered
 * @param xorValue the value to xor with
 */
void CPU::byteXOR(byte_t& reg, const byte_t& xorValue)
{
    reg ^= xorValue;

    registers.f.carry = false;
    registers.f.half_carry = false;
    registers.f.subtract = false;
    registers.f.zero = !reg;
}

/**
 * @brief compares passed values, 
 * effectively performs a subtraction of (reg - cmpValue) 
 *  and sets the appropriate flags
 * @param reg the reg being compared
 * @param cmpValue the value to compare with
 */
void CPU::byteCP(const byte_t& reg, const byte_t& cmpValue)
{
    signed int htest{ reg & 0xF };
    htest -= static_cast<signed int>(cmpValue & 0xF);

    registers.f.half_carry = htest < 0;
    registers.f.carry = reg < cmpValue;
    registers.f.zero = reg == cmpValue;
    registers.f.subtract = true;
}
/**
 * @brief Increments a passed byte, 
 * sets zero flag if zero, resets subtract flag,
 *  and sets half carry flag if carry from bit 3 occurs.
 * 
 * @param reg the byte to be incremented
 */
void CPU::byteINC(byte_t& reg)
{
    byte_t unchanged{ reg };
    ++reg;

    registers.f.zero = !reg;
    registers.f.subtract = false;
    registers.f.half_carry = unchanged==0xFu;
}
/**
 * @brief Decrements a passed byte, 
 * sets zero flag if zero, sets subtract flag,
 *  and sets half carry flag if borrow from bit 3 occurs.
 * 
 * @param reg the byte to be decremented
 */
void CPU::byteDEC(byte_t& reg)
{
    signed int htest{ reg & 0xF };
    --reg;

    registers.f.zero = !reg;
    registers.f.subtract = true;
    registers.f.half_carry = htest - 1 < 0;
}

/**
 * @brief add a word to the given reg, 
 * Subtract flag is reset, 
 * Carry flag set if carry from bit 15, Half Carry flag set if carry from bit 11.
 * @param reg the reg being altered
 * @param addValue the value to add with
 */
void CPU::wordAdd(word_t& reg, const word_t& addValue)
{
    unsigned int ctest{ static_cast<unsigned int>(reg)+static_cast<unsigned int>(addValue) };
    word_t htest{ (reg & 0xFFF) + (addValue & 0xFFF) };
    
    reg += addValue;
    
    registers.f.subtract = false;
    registers.f.carry = ctest > 0xFFFF;
    registers.f.half_carry = htest > 0xFFF;
}

