#include "inc/bitfuncs.h"

bool testBit(const byte_t& byte, int bit)
{
    return byte & (1 << bit);
}

void resetBit(byte_t& byte, int bit)
{
    byte &= ~(1 << bit);
}

void setBit(byte_t& byte, int bit)
{
    byte |= (1 << bit); 
}

int extractBits(const byte_t& byte, int start, int num)
{
    return (((1 << num) - 1) & (byte >> start));
}

