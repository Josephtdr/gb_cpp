#include "inc/cpu.h"

#include <iostream>
#include <stdexcept> // for std::runtime_error

/**
 * @brief Reads the next word from memory according to the pc, 
 * lsb first, also increments the pc twice.
 * @return byte_t 
 */
word_t CPU::readNextWord()
{
    byte_t lsb{ m_Memory.readByte(m_PC) };
    ++m_PC;
    byte_t msb{ m_Memory.readByte(m_PC) };
    ++m_PC;
    
    return (msb << 8) | lsb;
}
/**
 * @brief Reads the next byte from memory according to the pc, 
 * also increments the pc.
 * @return byte_t 
 */
byte_t CPU::readNextByte()
{
    byte_t out{ m_Memory.readByte(m_PC) };
    ++m_PC;
    return out;
}

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
    byte_t lsb{ static_cast<byte_t>(value & 0xFFu) };
    byte_t msb{ static_cast<byte_t>(value >> 8) };
    m_log(LOG_DEBUG) << "SP: " << +m_SP << ", will push " << +msb 
                     << ", " << +lsb << " to stack!" << "\n"; 

    --m_SP;
    m_Memory.writeByte(m_SP, msb);
    --m_SP;
    m_Memory.writeByte(m_SP, lsb);
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
    word_t out{ static_cast<word_t>((msb << 8) | lsb) };

    m_log(LOG_DEBUG) << "SP: " << +(m_SP-2) << ", Popped " << +out << " from stack!" << "\n"; 
    return out;
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

void CPU::jumpRelative(JumpTest type, const byte_t& unsignedData)
{
    int sign{ (unsignedData >> 7) };

    if (sign)
    {
        int offset{ (unsignedData & 0b01111111) - 0b10000000 };
        int intAddress{ m_PC + offset };

        word_t newAddress{ static_cast<word_t>(intAddress) };

        m_log(LOG_INFO) << "old address: " << +m_PC << ", new address: " << +newAddress 
            << ", offset: " << std::dec << offset << std::hex
                        <<".\n";

        jump(type, newAddress);

    }
    else
    {
        jump(type, m_PC + (unsignedData >> 1));
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


byte_t& CPU::getRegister(int index)
{
    static const std::vector<std::reference_wrapper<byte_t>> standardDataReg
    {
        m_Registers.b, m_Registers.c, m_Registers.d, m_Registers.e, m_Registers.h,
        m_Registers.l, m_Registers.a, m_Registers.a 
    }; //index 6 is filler! should not be used

    if (index==6)
        throw std::runtime_error("Invalid index 6 used in CPU::getRegister");

    return standardDataReg[index].get(); 
}

int CPU::cpu_restart(const byte_t& opcode)
{
    static const std::vector<byte_t> offsetVector
    {
        0x0u, 0x8u, 0x10u, 0x18u, 0x20u, 0x28u, 0x30u, 0x38u
    };
    int index{ (opcode /8) % 8 };

    push(m_PC);
    m_PC = (0x0000 + offsetVector[index]);

    return 16;
}

int CPU::cpu_byteArithmetic(const byte_t& opcode)
{
    using arithmeticFunctionPtr = void(CPU::*)(const byte_t&);
    static const std::vector<std::pair<arithmeticFunctionPtr, std::string>> arithmeticFunction
    {
        {&CPU::byteAdd, "ADD"}, {&CPU::byteAddWithCarry, "ADC"}, {&CPU::byteSub, "SUB"}, 
        {&CPU::byteSubWithCarry, "SBC"}, {&CPU::byteAND, "AND"}, {&CPU::byteXOR, "XOR"}, 
        {&CPU::byteOR, "OR"}, {&CPU::byteCP, "CP"}
    };

    byte_t data{};
    int ticks{ 4 };
    int dataIndex{ opcode % 8 };
    int functionIndex{ (static_cast<int>(opcode)/8) % 8 };
    m_log(LOG_INFO) << "Arithmetic Function of type: " << arithmeticFunction[functionIndex].second <<"\n";
    
    if (dataIndex == 6)
    {
        ticks += 4;
        if (opcode > 0xC0)
            data = readNextByte();
        else
            data = m_Memory.readByte(m_Registers.get_hl());
    }
    else
        data = getRegister(dataIndex);

    ((*this).*(arithmeticFunction[functionIndex].first))(data);
    return ticks;
}

void CPU::byteAdd(const byte_t& data)
{
    unsigned int ctest{ static_cast<unsigned int>(m_Registers.a)+static_cast<unsigned int>(data) };
    word_t htest{ static_cast<word_t>((m_Registers.a & 0xFu) + (data & 0xFu)) };
    
    m_Registers.a += data;
    
    m_Registers.f.zero = !m_Registers.a;
    m_Registers.f.subtract = false;
    m_Registers.f.carry = ctest > 0xFFu;
    m_Registers.f.half_carry = htest > 0xFu;
}
void CPU::byteAddWithCarry(const byte_t& data)
{
    byteAdd(data + static_cast<byte_t>(m_Registers.f.carry));
}
void CPU::byteSub(const byte_t& data)
{
    byte_t unchanged{ m_Registers.a };
    signed int htest{ m_Registers.a & 0xF };
    htest -= static_cast<signed int>(data & 0xF);

    m_Registers.a -= data;
    
    m_Registers.f.zero = !m_Registers.a;
    m_Registers.f.subtract = true;
    m_Registers.f.carry = unchanged < data ;
    m_Registers.f.half_carry = htest < 0;
}
void CPU::byteSubWithCarry(const byte_t& data)
{
    byteSub( data + static_cast<byte_t>(m_Registers.f.carry) );
}
void CPU::byteAND(const byte_t& data)
{
    m_Registers.a &= data;

    m_Registers.f.carry = false;
    m_Registers.f.half_carry = true;
    m_Registers.f.subtract = false;
    m_Registers.f.zero = !m_Registers.a;
}
void CPU::byteOR(const byte_t& data)
{
    m_Registers.a |= data;

    m_Registers.f.carry = false;
    m_Registers.f.half_carry = false;
    m_Registers.f.subtract = false;
    m_Registers.f.zero = !m_Registers.a;
}
void CPU::byteXOR(const byte_t& data)
{
    m_Registers.a ^= data;

    m_Registers.f.carry = false;
    m_Registers.f.half_carry = false;
    m_Registers.f.subtract = false;
    m_Registers.f.zero = !m_Registers.a;
}
void CPU::byteCP(const byte_t& data)
{
    signed int htest{ m_Registers.a & 0xF };
    htest -= static_cast<signed int>(data & 0xF);

    m_Registers.f.half_carry = htest < 0;
    m_Registers.f.carry = m_Registers.a < data;
    m_Registers.f.zero = m_Registers.a == data;
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

/**
 * @brief swap the upper and lower nibbles of the given value
 * 
 * @param reg 
 */
void CPU::swapNibbles(byte_t& reg)
{
    byte_t lsn{ static_cast<byte_t>(reg & 0xFu) };
    byte_t msn{ static_cast<byte_t>(reg >> 4) };

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
void CPU::testBit_OP(byte_t& byte, int bit)
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
void CPU::leftRotate(byte_t& byte)
{
    byte_t carry{ m_Registers.f.carry };
    byte_t oldBit7{ static_cast<byte_t>(byte >> 7) };

    m_Registers.f.carry = oldBit7;
    byte = (byte << 1) | oldBit7;    

    m_Registers.f.zero = !byte;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = false;
}

void CPU::leftRotateWithCarry(byte_t& byte)
{
    byte_t carry{ m_Registers.f.carry };
    byte_t oldBit7{ static_cast<byte_t>(byte >> 7) };

    m_Registers.f.carry = oldBit7;
    byte = (byte << 1) | carry;    

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
void CPU::rightRotate(byte_t& byte)
{
    byte_t carry{ m_Registers.f.carry };
    byte_t oldBit0{ static_cast<byte_t>(byte & 0b1) };

    m_Registers.f.carry = oldBit0;
    byte = (oldBit0 << 7) | (byte >> 1);
    
    m_Registers.f.zero = !byte;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = false;
}

void CPU::rightRotateWithCarry(byte_t& byte)
{
    byte_t carry{ m_Registers.f.carry };
    byte_t oldBit0{ static_cast<byte_t>(byte & 0b1) };

    m_Registers.f.carry = oldBit0;
    byte = (carry << 7) | (byte >> 1);

    m_Registers.f.zero = !byte;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = false;
}

void CPU::leftShift(byte_t& byte)
{
    byte_t oldBit7{ static_cast<byte_t>(byte >> 7) };

    m_Registers.f.carry = oldBit7;
    byte <<= 1;

    m_Registers.f.zero = !byte;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = false;
}
void CPU::rightShift(byte_t& byte)
{
    byte_t oldBit0{ static_cast<byte_t>(byte & 0b1) };
    byte_t oldBit7{ static_cast<byte_t>(byte & 0b10000000) };

    m_Registers.f.carry = oldBit0;
    byte >>= 1;

    m_Registers.f.zero = !byte;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = false;
}

void CPU::rightShiftArithmetic(byte_t& byte)
{
    byte_t oldBit0{ static_cast<byte_t>(byte & 0b1) };
    byte_t oldBit7{ static_cast<byte_t>(byte & 0b10000000) };

    m_Registers.f.carry = oldBit0;
    byte >>= 1;
    byte |= (oldBit7);    

    m_Registers.f.zero = !byte;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = false;
}

void CPU::checkDAA(byte_t& byte)
{
    byte_t lowerNibble{ static_cast<byte_t>(byte & 0xFu) };
    byte_t upperNibble{ static_cast<byte_t>(byte >> 4) };
    
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