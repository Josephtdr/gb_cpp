#pragma once

#include "consts.h"

bool testBit(const byte_t& byte, int bit);
void resetBit(byte_t& byte, int bit);
void setBit(byte_t& byte, int bit);
int extractBits(const byte_t& byte, int start, int num);

