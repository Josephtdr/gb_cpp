#ifndef H_REGISTERS
#define H_REGISTERS

#include "consts.h"

const unsigned int c_ZERO_FLAG_BYTE_POS{ 7 };
const unsigned int c_SUBTRACT_FLAG_BYTE_POS{ 6 };
const unsigned int c_HALFCARRY_FLAG_BYTE_POS{ 5 };
const unsigned int c_CARRY_FLAG_BYTE_POS{ 4 };

class FlagsRegister 
{
private: 
    bool zero{};
    bool subtract{};
    bool half_carry{};
    bool carry{};
    
public:
    FlagsRegister() = default;

    FlagsRegister(byte_t value);
    
    operator byte_t() const;
};

class Registers
{
public:
    byte_t a{};
    byte_t b{};
    byte_t c{};
    byte_t d{};
    byte_t e{};
    FlagsRegister f{};
    byte_t g{};
    byte_t h{};
    byte_t l{};

    word_t get_af() const;
    void set_af(word_t value);
    word_t get_bc() const;
    void set_bc(word_t value);
    word_t get_de() const;
    void set_de(word_t value);
    word_t get_hl() const;
    void set_hl(word_t value);
};

#endif