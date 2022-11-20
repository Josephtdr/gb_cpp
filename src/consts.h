#ifndef H_CONSTS
#define H_CONSTS

#include <cstdint>

using byte_t = uint8_t;
using word_t = uint16_t;

const unsigned int c_MEMORY_SIZE = 0xFFFF;
const unsigned int c_INSTRUCTION_TABLE_SIZE = 0xFF;
const unsigned int c_PREFIXED_INSTRUCTION_BYTE = 0xCB;

#endif