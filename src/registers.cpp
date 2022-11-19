#include "registers.h"

FlagsRegister::FlagsRegister(byte_t value)
    : zero{static_cast<bool>( (value >> c_ZERO_FLAG_BYTE_POS) & 0b1 )},
        subtract{static_cast<bool>( (value >> c_SUBTRACT_FLAG_BYTE_POS) & 0b1 )},
        half_carry{static_cast<bool>( (value >> c_HALFCARRY_FLAG_BYTE_POS) & 0b1 )},
        carry{static_cast<bool>( (value >> c_CARRY_FLAG_BYTE_POS) & 0b1 )}
{ }

FlagsRegister::operator byte_t() const 
{
    return (
        (zero ? 1u : 0u) << c_ZERO_FLAG_BYTE_POS |
        (subtract ? 1u : 0u) << c_SUBTRACT_FLAG_BYTE_POS |
        (half_carry ? 1u : 0u) << c_HALFCARRY_FLAG_BYTE_POS |
        (carry ? 1u : 0u) << c_CARRY_FLAG_BYTE_POS
    );
}

word_t Registers::get_af() const { return (a << 8) | f; }
word_t Registers::get_bc() const { return (b << 8) | c; }
word_t Registers::get_de() const { return (d << 8) | e; }
word_t Registers::get_hl() const { return (h << 8) | l; }

void Registers::set_af(word_t value) 
{
    f = (value & 0x00FF);
    a = (value >> 8);
}
void Registers::set_bc(word_t value) 
{
    c = (value & 0x00FF);
    b = (value >> 8);
}
void Registers::set_de(word_t value) 
{
    e = (value & 0x00FF);
    d = (value >> 8);
}
void Registers::set_hl(word_t value) 
{
    l = (value & 0x00FF);
    h = (value >> 8);
}