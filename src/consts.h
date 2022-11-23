#ifndef H_CONSTS
#define H_CONSTS

#include <cstdint>
#include <array>

using byte_t = uint8_t;
using word_t = uint16_t;

const unsigned int c_MEMORY_SIZE = 0xFFFF;
const unsigned int c_CARTRIDGE_MEMORY_SIZE = 0x200000;
const unsigned int c_RAM_BANKS_MEMORY_SIZE = 0x8000;
const unsigned int c_ROM_BANK_SIZE = 0x4000;
const unsigned int c_RAM_BANK_SIZE = 0x2000;

const unsigned int c_INSTRUCTION_TABLE_SIZE = 0xFF;
const unsigned int c_PREFIXED_INSTRUCTION_BYTE = 0xCB;
const unsigned int c_TOP_OF_STACK = 0xFFFE;
const unsigned int c_INITIAL_PC_VALUE = 0x100u;

const int c_MAX_CYCLES_PER_UPDATE = 69905;
const int m_CLOCKSPEED = 4194304;
const unsigned int c_TIMA = 0xFF05; // timer
const unsigned int c_TMA = 0xFF06; // timer modulator
const unsigned int c_TMC = 0xFF07; // timer controller
const unsigned int c_DIV_REGISTER = 0xFF04; // Divider Register
const int c_DIVIDER_CYCLE_FREQ = 256; // Divider register update frequency in cycles
#endif