#include "inc/cpu.h"

#include <iostream>
#include <stdexcept> // for std::runtime_error
#include <functional> // for std::reference_wrapper
#include <vector> 

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

/**
 * @brief Adds the given value to the stack, a byte at a time, msb first, 
 * also decrements the sp twice.
 * @param value the value to be added to the stack
 */
void CPU::push(word_t value)
{
    word_t lsb{ static_cast<word_t>(value & 0xFFu) };
    word_t msb{ static_cast<word_t>(value >> 8) };
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

    word_t out{ (msb << 8) | lsb };

    m_log(LOG_DEBUG) << "SP: " << +(m_SP-2) << ", Popped " << +out << " from stack!" << "\n"; 
    return out;
}

void CPU::popAF()
{
    push(m_Registers.get_af());
    m_log(LOG_ERROR) << "Opcode 0xF1 needs to be affect flags" << "\n";
    std::exit(EXIT_FAILURE);
}

int CPU::cpu_wordPopPush(const byte_t& opcode)
{
    int type{ opcode%8 };
    int reg{ (static_cast<int>(opcode)/8)%8 };

    switch (reg)
    {
        case 0:
            (type==1) ? m_Registers.set_bc(pop()) : push(m_Registers.get_bc()); break;
        case 2:
            (type==1) ? m_Registers.set_de(pop()) : push(m_Registers.get_de()); break;
        case 4:
            (type==1) ? m_Registers.set_hl(pop()) : push(m_Registers.get_hl()); break;
        case 6:
            (type==1) ? popAF() : m_Registers.set_af(pop()); break;
        default:
            throw std::runtime_error("invalid pop or push!");
    } 
    return (type==1) ? 12 : 16;
}

bool CPU::testJumpTest(int type)
{
    switch (type)
    {
        case 0: return !m_Registers.f.zero; break;
        case 1: return m_Registers.f.zero; break;
        case 2: return !m_Registers.f.carry; break;
        case 3: return m_Registers.f.carry; break;
        case 4: return true; break;
        default:
            throw std::runtime_error("Invalid jump type!");
    }
}

int CPU::cpu_jumpRelative(const byte_t& opcode)
{
    byte_t unsignedData{ readNextByte() };
    int sign{ (unsignedData >> 7) };
    int type{ static_cast<int>(opcode)/8};
    type = (type==3) ? 4 : type%4; 
    word_t newAddress{};

    if (sign)
    {
        int offset{ (unsignedData & 0b01111111) - 0b10000000 };
        int intAddress{ m_PC + offset };

        newAddress = static_cast<word_t>(intAddress);

        m_log(LOG_INFO) << "old address: " << +m_PC << ", new address: " << +newAddress 
            << ", offset: " << std::dec << offset << std::hex
                        <<".\n";
    }
    else
    {
        newAddress = m_PC + (unsignedData % 0b01111111);   
    }

    if (testJumpTest(type))
    {
        m_PC = newAddress;
        return 12;
    }
    return 8;
}

int CPU::cpu_jump(const byte_t& opcode)
{
    bool always{ opcode % 2 };
    int test{ ((static_cast<int>(opcode)/8)%8) /2 };

    int type{ static_cast<int>(opcode % 8)/2 };

    switch(type)
    {
        case 0: return return_(test); break;
        case 1: return jump(test); break;
        case 2: return call(test); break;
    }
}

int CPU::jump(int type)
{   
    word_t address{ readNextWord() };

    if (testJumpTest(type))
    {
        m_PC = address;
        return 16;
    }
    return 12;
}

int CPU::call(int type)
{
    word_t address{ readNextWord() };
    if (testJumpTest(type))
    {
        push(m_PC);
        m_PC = address;
        return 24;
    }
    return 12;
}

int CPU::return_(int type)
{
    if (testJumpTest(type))
    { 
        m_PC = pop();
        return (type < 4) ? 20 : 16;
    }
    return 8;
}

int CPU::cpu_wordLoad(const byte_t& opcode)
{
    word_t data{ readNextWord() };

    switch(opcode & 0xF0)
    {
        case 0x00: m_Registers.set_bc(data); break;
        case 0x10: m_Registers.set_de(data); break;
        case 0x20: m_Registers.set_hl(data); break;
        case 0x30: m_SP = data; break;
        case 0xF0: m_SP = m_Registers.get_hl(); break;
        default:
            throw std::runtime_error("BAD cpu_wordLoad!");
    }
    return 12;
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

int CPU::cpu_byteLoad(const byte_t& opcode)
{
    int dataIndex{ opcode % 8 };
    int targetIndex{ (static_cast<int>(opcode)/8) % 8 };
    byte_t data{};
    int ticks{ 4 };

    if (dataIndex == 6)
    {
        ticks += 4;
        if (opcode < 0x40)
            data = readNextByte();
        else
            data = m_Memory.readByte(m_Registers.get_hl());
    } 
    else
    {
        data = getRegister(dataIndex);
    }

    if (targetIndex == 6)
    {
        ticks += 4;
        m_Memory.writeByte(m_Registers.get_hl(), data);
    } 
    else 
    {
        getRegister(targetIndex) = data;
    }

    return ticks;
}

int CPU::cpu_byteIndexLoad(const byte_t& opcode)
{
    int target{ (static_cast<int>(opcode)/8) % 4 };
    int aIsSource{  (static_cast<int>(opcode)/8) % 2 };

    byte_t data{};
    if (aIsSource)
    {
        data = m_Registers.a;

        switch(target)
        {
            case 0: m_Memory.writeByte(m_Registers.get_bc(), data); break;
            case 1: m_Memory.writeByte(m_Registers.get_de(), data); break;
            case 2: 
                m_Memory.writeByte(m_Registers.get_hl(), data); 
                m_Registers.set_hl(m_Registers.get_hl()+1);
            break;
            case 3: 
                m_Memory.writeByte(m_Registers.get_hl(), data); 
                m_Registers.set_hl(m_Registers.get_hl()-1);
            break;
            default:
                throw std::runtime_error("Invalid target in cpu_byteIndexLoad!");
        }
    }
    else
    {
        switch(target)
        {
            case 0: data = m_Memory.readByte(m_Registers.get_bc()); break;
            case 1: data = m_Memory.readByte(m_Registers.get_de()); break;
            case 2: 
                data = m_Memory.readByte(m_Registers.get_hl()); 
                m_Registers.set_hl(m_Registers.get_hl()+1);
            break;
            case 3: 
                data = m_Memory.readByte(m_Registers.get_hl()); 
                m_Registers.set_hl(m_Registers.get_hl()-1);
            break;
            default:
                throw std::runtime_error("Invalid target in cpu_byteIndexLoad!");
        }
        m_Registers.a = data;
    }
    return 8;
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

int CPU::cpu_WordIncDec(const byte_t& opcode)
{
    int index{ (static_cast<int>(opcode)/8) };
    int value{ (index%2) ? 1 : - 1 };

    switch(index)
    {
        case 0: case 1:
            m_Registers.set_bc( m_Registers.get_bc() + value ); break;
        case 2: case 3:
            m_Registers.set_de( m_Registers.get_de() + value ); break;
        case 4: case 5:
            m_Registers.set_hl( m_Registers.get_hl() + value ); break;
        case 6: case 7:
            m_SP += value; break;
    }

    return 8;
}

int CPU::cpu_byteIncDec(const byte_t& opcode)
{
    int regIndex{ (static_cast<int>(opcode)/8) % 8 };
    int type{ opcode % 8 };

    if (regIndex==6)
    {
        byte_t HLI{ m_Memory.readByte(m_Registers.get_hl()) };
        (type == 4) ? byteINC(HLI) : byteDEC(HLI);
        m_Memory.writeByte(m_Registers.get_hl(), HLI);
        return 12;
    }
    else
    {
        (type == 4) ? byteINC(getRegister(regIndex)) : byteDEC(getRegister(regIndex));
        return 4;
    }
}

void CPU::byteINC(byte_t& reg)
{
    byte_t unchanged{ reg };
    ++reg;

    m_Registers.f.zero = !reg;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = unchanged==0xFu;
}

void CPU::byteDEC(byte_t& reg)
{
    signed int htest{ reg & 0xF };
    --reg;

    m_Registers.f.zero = !reg;
    m_Registers.f.subtract = true;
    m_Registers.f.half_carry = htest - 1 < 0;
}

int CPU::cpu_wordAdd(const byte_t& opcode)
{
    int source{ (static_cast<int>(opcode)/8)};

    word_t HL{ m_Registers.get_hl() };

    switch(opcode & 0xF0)
    {
        case 0x00: wordAdd(HL, m_Registers.get_bc()); break;
        case 0x10: wordAdd(HL, m_Registers.get_de()); break;
        case 0x20: wordAdd(HL, m_Registers.get_hl()); break;
        case 0x30: wordAdd(HL, m_SP); break;
        default:
            throw std::runtime_error("BAD cpu_wordADD!");
    }
    m_Registers.set_hl(HL);
    return 8;
}

void CPU::wordAdd(word_t& reg, const word_t& data)
{
    unsigned int ctest{ static_cast<unsigned int>(reg)+static_cast<unsigned int>(data) };
    word_t htest{ static_cast<word_t>((reg & 0xFFF) + (data & 0xFFF)) };
    
    reg += data;
    
    m_Registers.f.subtract = false;
    m_Registers.f.carry = ctest > 0xFFFF;
    m_Registers.f.half_carry = htest > 0xFFF;
}

int CPU::cpu_byteArithmetic(const byte_t& opcode)
{
    using arithmeticFunctionPtr = void(CPU::*)(const byte_t&);
    static const std::vector<arithmeticFunctionPtr> arithmeticFunction
    {
        &CPU::byteAdd, &CPU::byteAddWithCarry, &CPU::byteSub, &CPU::byteSubWithCarry,
        &CPU::byteAND, &CPU::byteXOR, &CPU::byteOR, &CPU::byteCP
    };

    int dataIndex{ opcode % 8 };
    int functionIndex{ (static_cast<int>(opcode)/8) % 8 };
    byte_t data{};
    int ticks{ 4 };

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

    ((*this).*(arithmeticFunction[functionIndex]))(data);
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

int CPU::cpu_checkDAA(const byte_t&)
{
    byte_t lowerNibble{ static_cast<byte_t>(m_Registers.a & 0xFu) };
    byte_t upperNibble{ static_cast<byte_t>(m_Registers.a >> 4) };
    
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
    m_Registers.a = (upperNibble << 4) | lowerNibble;

    m_Registers.f.zero = !m_Registers.a;
    m_Registers.f.half_carry = false;

    return 4;
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
