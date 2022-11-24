#include "cpu.h"

#include <stdexcept> // for std::runtime_error

CPU::CPU()
 : m_PC{ c_INITIAL_PC_VALUE }, m_SP{ c_TOP_OF_STACK }
{
    m_Registers.set_af(0x01B0);
    m_Registers.set_bc(0x0013);
    m_Registers.set_de(0x00D8);
    m_Registers.set_hl(0x014D);
}

int CPU::cycle()
{
    if (halted)
        return 1; //not sure what return value should be here, check interupts

    byte_t instructionByte{ m_Memory.readByte(m_PC) };

    bool prefixed{ instructionByte == c_PREFIXED_INSTRUCTION_BYTE };
    if (prefixed)
    { 
        ++m_PC;
        instructionByte = m_Memory.readByte(m_PC); 
    }
    ++m_PC;
    int nCycles{ execute(instructionByte, prefixed) };

    return nCycles;
}
int CPU::execute(byte_t instructionByte, bool prefixed)
{
    if (!prefixed)
        { return ((*this).*(instructionTable[instructionByte]))(); }
    else
        { return ((*this).*(prefixedInstructionTable[instructionByte]))(); }
}

/**
 * @brief Reads the next word from memory according to the pc, 
 * lsb first, also increments the pc twice.
 * @return byte_t 
 */
word_t CPU::readNextWord()
{
    byte_t lsb{ m_Memory.readByte(m_PC++) };
    byte_t msb{ m_Memory.readByte(m_PC++) };
    return (msb << 8) | lsb;
}
/**
 * @brief Reads the next byte from memory according to the pc, 
 * also increments the pc.
 * @return byte_t 
 */
byte_t CPU::readNextByte()
{
    return m_Memory.readByte(m_PC++);
}

void CPU::halt()
{
    halted = true;
}

/**
 * @brief swap the upper and lower nibbles of the given value
 * 
 * @param reg 
 */
void CPU::swapNibbles(byte_t& reg)
{
    byte_t lsn{ reg & 0xFu };
    byte_t msn{ reg >> 4 };

    reg = ((lsn << 4) | msn); 

    m_Registers.f.zero = !reg;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = false;
    m_Registers.f.carry = false;
}

/**
 * @brief performs the function testBit, but with the intention of setting
 * the f register, if testBit is false, the zero register is set. 
 * 
 * @param byte the byte
 * @param bit the nth bit from the right, starting at 0
 */
void CPU::testBit_OP(const byte_t& byte, int bit)
{
    bool test{ testBit(byte, bit) };

    m_Registers.f.zero = !test;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = true;
}

/**
 * @brief returns true if the bit is set in the byte
 * 
 * @param byte the byte
 * @param bit the nth bit from the right, starting at 0
 */
bool CPU::testBit(const byte_t& byte, int bit) const
{
    return byte & (1 << bit);
}

/**
 * @brief sets a bit in a byte
 * 
 * @param byte the byte to be changed
 * @param bit the nth bit from the right, starting at 0
 */
void CPU::setBit(byte_t& byte, int bit)
{
    byte |= (1 << bit); 
}

/**
 * @brief resets a bit in a byte
 * 
 * @param byte the byte to be changed
 * @param bit the nth bit from the right, starting at 0
 */
void CPU::resetBit(byte_t& byte, int bit)
{
    byte &= ~(1 << bit);
}

/**
 * @brief Rotates the given byte left by 1.
 * 
 * @param byte 
 * @param withCarry If true, then rotated through the carry, 
 * (i.e. bit 7 becomes the carry and the carry becomes bit 0),
 * otherwise rotated normally but the carry is also set
 * to old bit 7. 
 */
void CPU::leftRotate(byte_t& byte, bool withCarry)
{
    byte_t carry{ m_Registers.f.carry };
    byte_t oldBit7{ (byte & 0b10000000) >> 7 };

    if (withCarry)
    {
        m_Registers.f.carry = oldBit7;
        byte = (byte << 1) | carry;
    } 
    else
    {
        m_Registers.f.carry = oldBit7;
        byte = (byte << 1) | oldBit7;
    }

    m_Registers.f.zero = !byte;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = false;
}

/**
 * @brief Rotates the given byte right by 1.
 * 
 * @param byte 
 * @param withCarry If true, then rotated through the carry, 
 * (i.e. bit 0 becomes the carry and the carry becomes bit 7),
 * otherwise rotated normally but the carry is also set
 * to old bit 0. 
 */
void CPU::rightRotate(byte_t& byte, bool withCarry)
{
    byte_t carry{ m_Registers.f.carry };
    byte_t oldBit0{ (byte & 0b1) };

    if (withCarry)
    {
        m_Registers.f.carry = oldBit0;
        byte = (carry << 7) | (byte >> 1);
    } 
    else
    {
        m_Registers.f.carry = oldBit0;
        byte = (oldBit0 << 7) | (byte >> 1);
    }

    m_Registers.f.zero = !byte;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = false;
}

void CPU::leftShift(byte_t& byte)
{
    byte_t oldBit7{ (byte & 0b10000000) >> 7 };

    m_Registers.f.carry = oldBit7;
    byte <<= 1;

    m_Registers.f.zero = !byte;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = false;
}
void CPU::rightShift(byte_t& byte, bool arithmeticShift)
{
    byte_t oldBit0{ (byte & 0b1) };
    byte_t oldBit7{ (byte & 0b10000000) };

    m_Registers.f.carry = oldBit0;
    byte >>= 1;

    if (arithmeticShift)
    {
        byte |= (oldBit7);
    }

    m_Registers.f.zero = !byte;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = false;
}

void CPU::checkDAA(byte_t& byte)
{
    byte_t lowerNibble{ byte & 0xFu };
    byte_t upperNibble{ (byte & 0xF0u) >> 4 };
    
    if (lowerNibble > 9u)
    {
        lowerNibble += 6u;
        lowerNibble &= 0xFu;
        upperNibble += 1u;
    }
    if (upperNibble > 9u)
    {
        upperNibble += 6u;
        upperNibble &= 0xFu;
        m_Registers.f.carry = true;
    }
    else
    {
        m_Registers.f.carry = false;
    }
    byte = (upperNibble << 4) | lowerNibble;

    m_Registers.f.zero = !byte;
    m_Registers.f.half_carry = false;
}


/**
 * @brief Adds the given value to the stack, a byte at a time, msb first, 
 * also decrements the sp twice.
 * @param value the value to be added to the stack
 */
void CPU::push(word_t value)
{
    --m_SP;
    m_Memory.writeByte(m_SP, ((value & 0xFF00u) >> 8));
    --m_SP;
    m_Memory.writeByte(m_SP, (value & 0xFFu));
}

/**
 * @brief Pops a value of the top of the stack 
 * and increments the sp twice
 * @return word_t the value from the top of the stack
 */
word_t CPU::pop()
{
    byte_t lsb{ m_Memory.readByte(m_SP) };
    ++m_SP;
    byte_t msb{ m_Memory.readByte(m_SP) };
    ++m_SP;
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
        push(m_PC);
        m_PC = address;
    }
}

/**
 * @brief Checks if the return is valid, if so 
 * pops the value from the top of the stack and sets the pc to it
 * otherwise does nothing.
 * 
 * @param valid jumptest type to check if the return is valid
 */
void CPU::return_(JumpTest type)
{
    if (testJumpTest(type))
        { m_PC = pop(); }
}

/**
 * @brief add the current pc to the stack then
 * resets the program counter to 0x0000 + the given address value
 * 
 * @param address a byte
 */
void CPU::restart(byte_t address)
{
    push(m_PC);
    m_PC = (0x0000 + address);
}

/**
 * @brief return whether the given jumptest is currently valid or not
 * 
 * @param type the type of jump being tested
 */
bool CPU::testJumpTest(JumpTest type)
{
    switch (type)
    {
        case JumpTest::Zero:
            return m_Registers.f.zero; break;
        case JumpTest::NotZero:
            return !m_Registers.f.zero; break;
        case JumpTest::Carry:
            return m_Registers.f.carry; break;
        case JumpTest::NotCarry:
            return !m_Registers.f.carry; break;
        case JumpTest::Always:
            return true; break;
        default:
            throw std::runtime_error("Invalid jump type!");
    }
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
        { m_PC = address; }
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
            value = m_Registers.a; break;
        case ByteLoadSource::B:
            value = m_Registers.b; break;
        case ByteLoadSource::C:
            value = m_Registers.c; break;
        case ByteLoadSource::D:
            value = m_Registers.d; break;
        case ByteLoadSource::E:
            value = m_Registers.e; break;
        case ByteLoadSource::H:
            value = m_Registers.h; break;
        case ByteLoadSource::L:
            value = m_Registers.l; break;
        case ByteLoadSource::D8:
            value = readNextByte(); break;
        case ByteLoadSource::BCI:
            value = m_Memory.readByte(m_Registers.get_bc()); break;
        case ByteLoadSource::DEI:
            value = m_Memory.readByte(m_Registers.get_de()); break;
        case ByteLoadSource::HLI:
            value = m_Memory.readByte(m_Registers.get_hl()); break;
        case ByteLoadSource::D16I:
            value = m_Memory.readByte(readNextWord()); break;
        case ByteLoadSource::FF00pC:
            value = m_Memory.readByte(0xFF00u+m_Registers.c); break;
        case ByteLoadSource::FF00pD8:
            value = m_Memory.readByte(0xFF00u+readNextByte()); break;
        default:
            throw std::runtime_error("Invalid load source!");
    }
    //D16I, BCI, DEI, HLI,
    switch (ldTarget)
    {
        case ByteLoadTarget::A:
            m_Registers.a = value; break;
        case ByteLoadTarget::B:
            m_Registers.b = value; break;
        case ByteLoadTarget::C:
            m_Registers.c = value; break;
        case ByteLoadTarget::D:
            m_Registers.d = value; break;
        case ByteLoadTarget::E:
            m_Registers.e = value; break;
        case ByteLoadTarget::H:
            m_Registers.h = value; break;
        case ByteLoadTarget::L:
            m_Registers.l = value; break;
        case ByteLoadTarget::BCI:
            m_Memory.writeByte(m_Registers.get_bc(), value); break;
        case ByteLoadTarget::DEI:
            m_Memory.writeByte(m_Registers.get_de(), value); break;
        case ByteLoadTarget::HLI:
            m_Memory.writeByte(m_Registers.get_hl(), value); break;
        case ByteLoadTarget::D16I:
            m_Memory.writeByte(readNextWord(), value); break;
        case ByteLoadTarget::FF00pC:
            m_Memory.writeByte(0xFF00u+m_Registers.c, value); break;
        case ByteLoadTarget::FF00pD8:
            m_Memory.writeByte(0xFF00u+readNextByte(), value); break;
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
            value = m_Registers.get_hl(); break;
        case WordLoadSource::SP:
            value = m_SP; break;
        case WordLoadSource::D16:
            value = readNextWord(); break;
        case WordLoadSource::SPpD8:
            {
                byte_t d8{ readNextByte() };
                m_Registers.f.zero = false;
                m_Registers.f.subtract = false;

                unsigned int v{ static_cast<unsigned int>(m_SP)+static_cast<unsigned int>(d8) };
                bool carry{ v > 0xFFFF };
                m_Registers.f.carry = carry;

                bool half_carry{ ((m_SP & 0xF) + (d8 & 0xF)) > 0xF };
                m_Registers.f.half_carry = half_carry;

                value = m_SP+d8; 
                break;
            }
        default:
            throw std::runtime_error("Invalid load source!");
    } 

    switch(ldTarget)
    {
        case WordLoadTarget::BC:
            m_Registers.set_bc(value); break;
        case WordLoadTarget::DE:
            m_Registers.set_de(value); break;
        case WordLoadTarget::HL:
            m_Registers.set_hl(value); break;
        case WordLoadTarget::SP:
            m_SP = value; break;
        case WordLoadTarget::D16I:
            m_Memory.writeByte(readNextWord(), value); break;
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
void CPU::byteAdd(byte_t& reg, const byte_t& addValue, bool withCarry)
{
    byte_t value{ addValue };
    if (withCarry)
        { value += static_cast<byte_t>(m_Registers.f.carry); }

    unsigned int ctest{ static_cast<unsigned int>(reg)+static_cast<unsigned int>(value) };
    word_t htest{ static_cast<word_t>((reg & 0xFu) + (value & 0xFu)) };
    
    reg += value;
    
    m_Registers.f.zero = !reg;
    m_Registers.f.subtract = false;
    m_Registers.f.carry = ctest > 0xFFu;
    m_Registers.f.half_carry = htest > 0xFu;
}
    
/**
 * @brief subtract a byte from the given reg, 
 * Zero flag set if result is 0, subtract flag is set, 
 * Carry flag set if no borrow, Half Carry flag set if borrow from bit 4.
 * @param reg the reg being altered
 * @param subValue the value to subtract with
 * @param withCarry include the carry flag in the subtraction
 */
void CPU::byteSub(byte_t& reg, const byte_t& subValue, bool withCarry)
{
    byte_t unchanged{ reg };
    byte_t value{ subValue };
    if (withCarry)
        { value += static_cast<byte_t>(m_Registers.f.carry); }

    signed int htest{ reg & 0xF };
    htest -= static_cast<signed int>(value & 0xF);
    reg -= value;
    
    m_Registers.f.zero = !reg;
    m_Registers.f.subtract = true;
    m_Registers.f.carry = unchanged < value ;
    m_Registers.f.half_carry = htest < 0;
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

    m_Registers.f.carry = false;
    m_Registers.f.half_carry = true;
    m_Registers.f.subtract = false;
    m_Registers.f.zero = !reg;
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

    m_Registers.f.carry = false;
    m_Registers.f.half_carry = false;
    m_Registers.f.subtract = false;
    m_Registers.f.zero = !reg;
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

    m_Registers.f.carry = false;
    m_Registers.f.half_carry = false;
    m_Registers.f.subtract = false;
    m_Registers.f.zero = !reg;
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

    m_Registers.f.half_carry = htest < 0;
    m_Registers.f.carry = reg < cmpValue;
    m_Registers.f.zero = reg == cmpValue;
    m_Registers.f.subtract = true;
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

    m_Registers.f.zero = !reg;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = unchanged==0xFu;
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

    m_Registers.f.zero = !reg;
    m_Registers.f.subtract = true;
    m_Registers.f.half_carry = htest - 1 < 0;
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
    word_t htest{ static_cast<word_t>((reg & 0xFFF) + (addValue & 0xFFF)) };
    
    reg += addValue;
    
    m_Registers.f.subtract = false;
    m_Registers.f.carry = ctest > 0xFFFF;
    m_Registers.f.half_carry = htest > 0xFFF;
}

