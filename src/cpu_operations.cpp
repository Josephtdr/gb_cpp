#include "inc/cpu.h"
#include "inc/bitfuncs.h"

#include <iostream>
#include <stdexcept> // for std::runtime_error
#include <cassert> // for assert
/**
 * @brief Reads the next word from memory according to the pc, 
 * lsb first, also increments the pc twice.
 * @return word_t 
 */
word_t CPU::readNextWord()
{
    byte_t lsb{ readByte(m_PC++) };
    byte_t msb{ readByte(m_PC++) };
    
    return (msb << 8) | lsb;
}
/**
 * @brief Reads the next byte from memory according to the pc, 
 * also increments the pc.
 * @return byte_t 
 */
byte_t CPU::readNextByte()
{
    byte_t out{ readByte(m_PC++) };
    return out;
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

    writeByte(--m_SP, msb);
    writeByte(--m_SP, lsb);
}

/**
 * @brief Pops a value of the top of the stack 
 * and increments the sp twice
 * @return word_t the value from the top of the stack
 */
word_t CPU::pop()
{
    byte_t lsb{ readByte(m_SP++) };
    byte_t msb{ readByte(m_SP++) };

    return (msb << 8) | lsb;
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
    }
}

int CPU::cpu_jumpRelative(byte_t opcode)
{
    byte_t unsignedData{ readNextByte() };
    int type{ ((opcode/8)-4) };
    if (type < 0) type = 4;

    word_t newAddress = signedAddition(m_PC, unsignedData);
    logOpcode((m_PC-2), opcode, unsignedData, 0x0, "JR", getJumpTestStr(type), "$"+wordStr(newAddress));

    if (testJumpTest(static_cast<JumpTest>(type)))
    {
        m_PC = newAddress;
        return 12;
    }
    return 8;
}

int CPU::cpu_jump(byte_t opcode)
{
    int funcIdx{ extractBits(opcode,1,2) };
    assert((funcIdx>=0&&funcIdx<3) && "Incorrect funcidx in cpu_jump!");
    int testType{ testBit(opcode,0) ? 4 : extractBits(opcode,3,2) }; // check if always true
    
    switch(funcIdx)
    {
        case 0: return return_(testType, opcode); break;
        case 1: return jump(testType, opcode); break;
        case 2: return call(testType, opcode); break;
    }
}

std::string_view CPU::getJumpTestStr(int type)
{
    static const std::vector<std::string> jumpTypes
    {
        "NZ", "Z", "NC", "C", ""
    };
    assert((type>=0&&type<=4) && "Incorrect type in getJumpTestStr!");
    return jumpTypes[type];
}

int CPU::jump(int type, byte_t opcode)
{   
    word_t address{ readNextWord() };
    logOpcode((m_PC-3), opcode, 0x0, 0x0, "JP", getJumpTestStr(type), "$"+wordStr(address));

    if (testJumpTest(static_cast<JumpTest>(type)))
    {
        m_PC = address;
        return 16;
    }
    return 12;
}
int CPU::call(int type, byte_t opcode)
{
    word_t address{ readNextWord() };
    logOpcode((m_PC-3), opcode, (address>>8), (address&0xFF), "CALL", getJumpTestStr(type), "$"+wordStr(address));

    if (testJumpTest(static_cast<JumpTest>(type)))
    {
        push(m_PC);
        m_PC = address;
        return 24;
    }
    return 12;
}
int CPU::return_(int type, byte_t opcode)
{
    logOpcode((m_PC-1), opcode, 0x0, 0x0, "RET", getJumpTestStr(type), "");

    if (testJumpTest(static_cast<JumpTest>(type)))
    { 
        m_PC = pop();
        return (static_cast<JumpTest>(type)!=JumpTest::Always) ? 20 : 16;
    }
    return 8;
}

word_t CPU::signedAddition(word_t target, byte_t unsignedData)
{
    word_t out{ target };
    out += static_cast<signed_byte_t>(unsignedData);
    
    return out;
}

byte_t& CPU::getRegister(int index)
{
    static const std::vector<std::reference_wrapper<byte_t>> standardDataReg
    {
        m_Registers.b, m_Registers.c, m_Registers.d, m_Registers.e, m_Registers.h,
        m_Registers.l, m_Registers.a, m_Registers.a 
    }; //index 6 is filler! should not be used
    assert((index >= 0 && index <= 7) && "incorrect index in getRegister!");
    assert((index!=6) && "Invalid index 6 used in CPU::getRegister");

    return standardDataReg[index].get(); 
}
std::string_view CPU::getRegisterStr(int index)
{
    static const std::vector<std::string> regString
    {
        "B", "C", "D", "E", "H", "L", "(HL)", "A"
    };
    assert((index >= 0 && index <= 7) && "incorrect index in getRegisterStr!");

    return regString[index]; 
}
std::string CPU::byteStr(byte_t byte)
{
    std::stringstream stream;
    stream << std::hex << std::setfill('0') << std::setw(2) << +byte;
    return stream.str();
}
std::string CPU::wordStr(word_t word)
{
    std::stringstream stream;
    stream << std::hex << std::setfill('0') << std::setw(4) << +word;
    return stream.str();
}


int CPU::cpu_restart(byte_t opcode)
{
    static const std::vector<byte_t> offsetVector
    {
        0x0u, 0x8u, 0x10u, 0x18u, 0x20u, 0x28u, 0x30u, 0x38u
    };
    int index{ extractBits(opcode,3,3) };
    assert((index >= 0 && index <= 7) && "incorrect index in cpu_restart!");

    logOpcode((m_PC-1), opcode, 0x0, 0x0, "RST", byteStr(offsetVector[index]), "");

    push(m_PC);
    m_PC = offsetVector[index];
    return 16;
}

int CPU::cpu_byteLoad(byte_t opcode)
{
    int dataIndex{ extractBits(opcode,0,3) };
    int targetIndex{ extractBits(opcode,3,3) };
    int ticks{4};
    byte_t data{};

    logOpcode((m_PC-1), opcode, 0x0, 0x0, "LD", getRegisterStr(targetIndex), ((opcode < 0x40) ? "u8" : getRegisterStr(dataIndex)));

    if (dataIndex == 6)
    {
        ticks += 4;
        if (opcode < 0x40)
            data = readNextByte();
        else
            data = readByte(m_Registers.get_hl());
    } 
    else
        data = getRegister(dataIndex);

    if (targetIndex == 6)
    {
        ticks += 4;
        writeByte(m_Registers.get_hl(), data);
    } 
    else 
        getRegister(targetIndex) = data;
    
    return ticks;
}

int CPU::cpu_byteArithmetic(byte_t opcode)
{
    static const std::vector<std::pair<std::function<void(byte_t)>, std::string>> arithmeticFunction
    {
        {std::bind(&CPU::byteAdd, this, std::placeholders::_1), "ADD"}, {std::bind(&CPU::byteAddWithCarry, this, std::placeholders::_1), "ADC"}, 
        {std::bind(&CPU::byteSub, this, std::placeholders::_1), "SUB"}, {std::bind(&CPU::byteSubWithCarry, this, std::placeholders::_1), "SBC"}, 
        {std::bind(&CPU::byteAND, this, std::placeholders::_1), "AND"}, {std::bind(&CPU::byteXOR, this, std::placeholders::_1), "XOR"}, 
        {std::bind(&CPU::byteOR, this, std::placeholders::_1), "OR"},   {std::bind(&CPU::byteCP, this, std::placeholders::_1), "CP"}
    };

    byte_t data{};
    int ticks{4};
    int dataIndex{ extractBits(opcode,0,3) };
    int functionIndex{ extractBits(opcode,3,3) };
    assert((functionIndex >= 0 && functionIndex <= 7) && "incorrect index in cpu_byteArithmetic!");

    logOpcode((m_PC-1), opcode, 0x0, 0x0, arithmeticFunction[functionIndex].second, "A", ((opcode > 0xC0) ? "u8" : getRegisterStr(dataIndex)));

    if (dataIndex == 6)
    {
        ticks += 4;
        if (opcode > 0xC0)
            data = readNextByte();
        else
            data = readByte(m_Registers.get_hl());
    }
    else
        data = getRegister(dataIndex);

    arithmeticFunction[functionIndex].first(data);
    return ticks;
}

void CPU::byteAdd(byte_t data)
{
    int ctest{ m_Registers.a + data };
    int htest{ (m_Registers.a&0xF) + (data&0xF) };

    m_Registers.a += data;
    
    m_Registers.f.zero = !m_Registers.a;
    m_Registers.f.subtract = false;
    m_Registers.f.carry = ctest >= 0x100;
    m_Registers.f.half_carry = htest >= 0x10;
}
void CPU::byteAddWithCarry(byte_t data)
{
    int ctest{ m_Registers.a + data + m_Registers.f.carry };
    int htest{ (m_Registers.a&0xF) + (data&0xF) + m_Registers.f.carry };
    
    m_Registers.a += (data+m_Registers.f.carry);
    
    m_Registers.f.zero = !m_Registers.a;
    m_Registers.f.subtract = false;
    m_Registers.f.carry = ctest >= 0x100;
    m_Registers.f.half_carry = htest >= 0x10;
}
void CPU::byteSub(byte_t data)
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
void CPU::byteSubWithCarry(byte_t data)
{
    byte_t unchanged{ m_Registers.a };
    int htest{ (m_Registers.a&0xF) - (data&0xF) - m_Registers.f.carry };

    m_Registers.a -= (data + m_Registers.f.carry);
    
    m_Registers.f.zero = !m_Registers.a;
    m_Registers.f.subtract = true;
    m_Registers.f.carry = unchanged < (data + m_Registers.f.carry) ;
    m_Registers.f.half_carry = htest < 0;
}
void CPU::byteAND(byte_t data)
{
    m_Registers.a &= data;

    m_Registers.f.carry = false;
    m_Registers.f.half_carry = true;
    m_Registers.f.subtract = false;
    m_Registers.f.zero = !m_Registers.a;
}
void CPU::byteOR(byte_t data)
{
    m_Registers.a |= data;

    m_Registers.f.carry = false;
    m_Registers.f.half_carry = false;
    m_Registers.f.subtract = false;
    m_Registers.f.zero = !m_Registers.a;
}
void CPU::byteXOR(byte_t data)
{
    m_Registers.a ^= data;

    m_Registers.f.carry = false;
    m_Registers.f.half_carry = false;
    m_Registers.f.subtract = false;
    m_Registers.f.zero = !m_Registers.a;
}
void CPU::byteCP(byte_t data)
{
    signed int htest{ m_Registers.a & 0xF };
    htest -= static_cast<signed int>(data & 0xF);

    m_Registers.f.half_carry = htest < 0;
    m_Registers.f.carry = m_Registers.a < data;
    m_Registers.f.zero = m_Registers.a == data;
    m_Registers.f.subtract = true;
}

int CPU::cpu_byteInc(byte_t opcode)
{
    int regIndex{ extractBits(opcode,3,3) };
    logOpcode((m_PC-1), opcode, 0x0, 0x0, "INC",getRegisterStr(regIndex), "");
    
    if (regIndex==6)
    {
        byte_t HLI{ readByte(m_Registers.get_hl()) };
        byteINC(HLI);
        writeByte(m_Registers.get_hl(), HLI);
        return 12;
    }
    else
    {
        byteINC(getRegister(regIndex));
        return 4;
    }
}

int CPU::cpu_byteDec(byte_t opcode)
{
    int regIndex{ extractBits(opcode,3,3) };
    logOpcode((m_PC-1), opcode, 0x0, 0x0, "DEC",getRegisterStr(regIndex), "");

    if (regIndex==6)
    {
        byte_t HLI{ readByte(m_Registers.get_hl()) };
        byteDEC(HLI);
        writeByte(m_Registers.get_hl(), HLI);
        return 12;
    }
    else
    {
        byteDEC(getRegister(regIndex));
        return 4;
    }
}

void CPU::byteINC(byte_t& reg)
{
    byte_t unchanged{ reg };
    ++reg;

    m_Registers.f.zero = !reg;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = (unchanged&0xF)==0xFu;
}

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
void CPU::wordAdd(word_t& reg, word_t addValue)
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
 */
void CPU::cpu_swapNibbles(byte_t& reg, int)
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
void CPU::cpu_testBit(byte_t& byte, int bit)
{
    bool test{ testBit(byte, bit) };

    m_Registers.f.zero = !test;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = true;
}

/**
 * @brief sets a bit in a byte
 * 
 * @param byte the byte to be changed
 * @param bit the nth bit from the right, starting at 0
 */
void CPU::cpu_setBit(byte_t& byte, int bit)
{
    setBit(byte, bit);
}

/**
 * @brief resets a bit in a byte
 * 
 * @param byte the byte to be changed
 * @param bit the nth bit from the right, starting at 0
 */
void CPU::cpu_resetBit(byte_t& byte, int bit)
{
    resetBit(byte, bit);
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
void CPU::cpu_leftRotate(byte_t& byte, int)
{
    byte_t carry{ m_Registers.f.carry };
    byte_t oldBit7{ static_cast<byte_t>(byte >> 7) };

    m_Registers.f.carry = oldBit7;
    byte = (byte << 1) | oldBit7;    

    m_Registers.f.zero = !byte;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = false;
}

void CPU::cpu_leftRotateWithCarry(byte_t& byte, int)
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
void CPU::cpu_rightRotate(byte_t& byte, int)
{
    byte_t carry{ m_Registers.f.carry };
    byte_t oldBit0{ static_cast<byte_t>(byte & 0b1) };

    m_Registers.f.carry = oldBit0;
    byte = (oldBit0 << 7) | (byte >> 1);
    
    m_Registers.f.zero = !byte;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = false;
}

void CPU::cpu_rightRotateWithCarry(byte_t& byte, int)
{
    byte_t carry{ m_Registers.f.carry };
    byte_t oldBit0{ static_cast<byte_t>(byte & 0b1) };

    m_Registers.f.carry = oldBit0;
    byte = (carry << 7) | (byte >> 1);

    m_Registers.f.zero = !byte;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = false;
}

void CPU::cpu_leftShift(byte_t& byte, int)
{
    byte_t oldBit7{ static_cast<byte_t>(byte >> 7) };

    m_Registers.f.carry = oldBit7;
    byte <<= 1;

    m_Registers.f.zero = !byte;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = false;
}
void CPU::cpu_rightShift(byte_t& byte, int)
{
    byte_t oldBit0{ static_cast<byte_t>(byte & 0b1) };
    byte_t oldBit7{ static_cast<byte_t>(byte & 0b10000000) };

    m_Registers.f.carry = oldBit0;
    byte >>= 1;

    m_Registers.f.zero = !byte;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = false;
}

void CPU::cpu_rightShiftArithmetic(byte_t& byte, int)
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

//Thank you to: https://forums.nesdev.org/viewtopic.php?t=15944
void CPU::checkDAA(byte_t& byte)
{
    if (!m_Registers.f.subtract) 
    {  // after an addition, adjust if (half-)carry occurred or if result is out of bounds
        if (m_Registers.f.carry || byte > 0x99) 
        { 
            byte += 0x60;
            m_Registers.f.carry = true;
        }
        if (m_Registers.f.half_carry || (byte & 0x0f) > 0x09) 
            byte += 0x6;
    } 
    else
    {  // after a subtraction, only adjust if (half-)carry occurred
        if (m_Registers.f.carry) 
            byte -= 0x60;
        if (m_Registers.f.half_carry)
            byte -= 0x6;
    }
    
    m_Registers.f.zero = !byte;
    m_Registers.f.half_carry = false;
}