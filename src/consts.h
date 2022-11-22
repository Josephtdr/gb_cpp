#ifndef H_CONSTS
#define H_CONSTS

#include <cstdint>

using byte_t = uint8_t;
using word_t = uint16_t;

const unsigned int c_MEMORY_SIZE = 0xFFFF;
const unsigned int c_INSTRUCTION_TABLE_SIZE = 0xFF;
const unsigned int c_PREFIXED_INSTRUCTION_BYTE = 0xCB;
const unsigned int c_TOP_OF_STACK = 0xFFFE;
const unsigned int c_INITIAL_PC_VALUE = 0x100u;

#endif