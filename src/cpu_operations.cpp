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
    byte_t lsb{ readByte(m_PC) };
    ++m_PC;
    byte_t msb{ readByte(m_PC) };
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
    byte_t out{ readByte(m_PC) };
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
    byte_t lsb{ static_cast<byte_t>(value & 0xFFu) };
    byte_t msb{ static_cast<byte_t>(value >> 8) };
    // m_log(LOG_DEBUG) << "SP: " << +m_SP << ", will push " << +msb 
    //                  << ", " << +lsb << " to stack!" << "\n"; 

    --m_SP;
    writeByte(m_SP, msb);
    --m_SP;
    writeByte(m_SP, lsb);
}

/**
 * @brief Pops a value of the top of the stack 
 * and increments the sp twice
 * @return word_t the value from the top of the stack
 */
word_t CPU::pop()
{
    byte_t lsb{ readByte(m_SP) };
    ++m_SP;
    byte_t msb{ readByte(m_SP) };
    ++m_SP;
    word_t out{ static_cast<word_t>((msb << 8) | lsb) };

    // m_log(LOG_DEBUG) << "SP: " << +(m_SP-2) << ", Popped " << +out << " from stack!" << "\n"; 
    return out;
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

int CPU::cpu_jumpRelative(const byte_t& opcode)
{
    byte_t unsignedData{ readNextByte() };
    int type{ ((opcode/8)-4) };
    if (type < 0) type = 4;

    m_log(LOG_INFO) << "PC: " << +m_PC << ", Opcode: 0x" << +opcode << ", JR " 
                    << getJumpTestStr(type) << " i8\n"; 

    if (testJumpTest(static_cast<JumpTest>(type)))
    {
        m_PC = unsignedAddition(m_PC, unsignedData);
        return 12;
    }
    return 8;
}

int CPU::cpu_jump(const byte_t& opcode)
{
    static const std::vector<std::string> funcStr
    {
        "RET", "JP", "CALL"
    };
    
    int funcIdx{ extractBits(opcode,1,2) };
    int testType{ testBit(opcode,0) ? 4 : extractBits(opcode,3,2) }; // check if always true

    m_log(LOG_INFO) << "PC: " << +m_PC << ", Opcode: 0x" << +opcode 
        << ", " << funcStr[funcIdx] << " " << getJumpTestStr(testType) 
        << ((!funcIdx) ? "" : "u16")  << "\n";
    switch(funcIdx)
    {
        case 0: return return_(static_cast<JumpTest>(testType)); break;
        case 1: return jump(static_cast<JumpTest>(testType)); break;
        case 2: return call(static_cast<JumpTest>(testType)); break;
        default:
            throw std::runtime_error("INVALID FUNCIDX IN CPU_JUMP");
    }
}

std::string_view CPU::getJumpTestStr(int type)
{
    static const std::vector<std::string> jumpTypes
    {
        "NZ", "Z", "NC", "C", ""
    };

    return jumpTypes[type];
}


int CPU::jump(JumpTest type)
{   
    word_t address{ readNextWord() };
    if (testJumpTest(type))
    {
        m_PC = address;
        return 16;
    }
    return 12;
}
int CPU::call(JumpTest type)
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
int CPU::return_(JumpTest type)
{
    if (testJumpTest(type))
    { 
        m_PC = pop();
        return (type!=JumpTest::Always) ? 20 : 16;
    }
    return 8;
}

word_t CPU::unsignedAddition(const word_t& target, const byte_t& unsignedData)
{
    int sign{ (unsignedData >> 7) };

    if (sign)
    {
        int offset{ (unsignedData & 0b01111111) - 0b10000000 };
        int intAddress{ target + offset };

        return static_cast<word_t>(intAddress);
    }
    else
    {
        return target + unsignedData;   
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
std::string_view CPU::getRegisterStr(int index)
{
    static const std::vector<std::string> regString
    {
        "B", "C", "D", "E", "H", "L", "(HL)", "A"
    };
    return regString[index]; 
}

int CPU::cpu_restart(const byte_t& opcode)
{
    static const std::vector<byte_t> offsetVector
    {
        0x0u, 0x8u, 0x10u, 0x18u, 0x20u, 0x28u, 0x30u, 0x38u
    };
    int index{ extractBits(opcode,3,3) };

    m_log(LOG_INFO) << "PC: " << +m_PC << ", Opcode: 0x" << +opcode << ", RST " 
                    << +offsetVector[index] << "\n"; 

    push(m_PC);
    m_PC = offsetVector[index];
    return 16;
}

int CPU::cpu_byteLoad(const byte_t& opcode)
{
    int dataIndex{ extractBits(opcode,0,3) };
    int targetIndex{ extractBits(opcode,3,3) };
    int ticks{4};
    byte_t data{};

    m_log(LOG_INFO) << "PC: " << +m_PC << ", Opcode: 0x" << +opcode << ", LD " 
                    << getRegisterStr(targetIndex) << " " 
                    << ((opcode < 0x40) ? "u8" : getRegisterStr(dataIndex)) << "\n"; 

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
    int ticks{4};
    int dataIndex{ extractBits(opcode,0,3) };
    int functionIndex{ extractBits(opcode,3,3) };

    m_log(LOG_INFO) << "PC: " << +m_PC << ", Opcode: 0x" << +opcode << ", "
                    << arithmeticFunction[functionIndex].second << " " 
                    << ((opcode > 0xC0) ? "u8" : getRegisterStr(dataIndex)) << "\n";

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

int CPU::cpu_byteInc(const byte_t& opcode)
{
    int regIndex{ extractBits(opcode,3,3) };

    m_log(LOG_INFO) << "PC: " << +m_PC << ", Opcode: 0x" << +opcode << ", INC " 
                    << getRegisterStr(regIndex) << "\n"; 

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

int CPU::cpu_byteDec(const byte_t& opcode)
{
    int regIndex{ extractBits(opcode,3,3) };

    m_log(LOG_INFO) << "PC: " << +m_PC << ", Opcode: 0x" << +opcode << ", DEC " 
                    << getRegisterStr(regIndex) << "\n"; 

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
void CPU::swapNibbles(byte_t& reg, int)
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
void CPU::leftRotate(byte_t& byte, int)
{
    byte_t carry{ m_Registers.f.carry };
    byte_t oldBit7{ static_cast<byte_t>(byte >> 7) };

    m_Registers.f.carry = oldBit7;
    byte = (byte << 1) | oldBit7;    

    m_Registers.f.zero = !byte;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = false;
}

void CPU::leftRotateWithCarry(byte_t& byte, int)
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
void CPU::rightRotate(byte_t& byte, int)
{
    byte_t carry{ m_Registers.f.carry };
    byte_t oldBit0{ static_cast<byte_t>(byte & 0b1) };

    m_Registers.f.carry = oldBit0;
    byte = (oldBit0 << 7) | (byte >> 1);
    
    m_Registers.f.zero = !byte;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = false;
}

void CPU::rightRotateWithCarry(byte_t& byte, int)
{
    byte_t carry{ m_Registers.f.carry };
    byte_t oldBit0{ static_cast<byte_t>(byte & 0b1) };

    m_Registers.f.carry = oldBit0;
    byte = (carry << 7) | (byte >> 1);

    m_Registers.f.zero = !byte;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = false;
}

void CPU::leftShift(byte_t& byte, int)
{
    byte_t oldBit7{ static_cast<byte_t>(byte >> 7) };

    m_Registers.f.carry = oldBit7;
    byte <<= 1;

    m_Registers.f.zero = !byte;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = false;
}
void CPU::rightShift(byte_t& byte, int)
{
    byte_t oldBit0{ static_cast<byte_t>(byte & 0b1) };
    byte_t oldBit7{ static_cast<byte_t>(byte & 0b10000000) };

    m_Registers.f.carry = oldBit0;
    byte >>= 1;

    m_Registers.f.zero = !byte;
    m_Registers.f.subtract = false;
    m_Registers.f.half_carry = false;
}

void CPU::rightShiftArithmetic(byte_t& byte, int)
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