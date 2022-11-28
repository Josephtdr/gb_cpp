#ifndef H_CONSTS
#define H_CONSTS

#include <cstdint>
#include <string>

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
const int c_DIVIDER_CYCLE_FREQ = 256; // Divider register update frequency in cycles

const unsigned int c_TIMA_ADDRESS = 0xFF05; // timer
const unsigned int c_TMA_ADDRESS = 0xFF06; // timer modulator
const unsigned int c_TMC_ADDRESS = 0xFF07; // timer controller
const unsigned int c_DIV_REGISTER_ADDRESS = 0xFF04; // Divider Register
const unsigned int c_INTERUPTS_REQ_ADDRESS = 0xFF0F;
const unsigned int c_INTERUPTS_ENABLED_ADDRESS = 0xFFFF;
const unsigned int c_VBLANK_INTERUPT = 0x40;
const unsigned int c_LCD_INTERUPT = 0x48;
const unsigned int c_TIMER_INTERUPT = 0x50;
const unsigned int c_JOYPAD_INTERUPT = 0x60;

const std::string c_BOOT_ROM_LOCATION = "/home/randa/Documents/emulation/gb_cpp/roms/DMG_ROM.bin";
const int c_BOOT_ROM_SIZE = 0x100;

#endif