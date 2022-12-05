#ifndef H_CONSTS
#define H_CONSTS

#include <cstdint>
#include <string>

using byte_t = uint8_t;
using word_t = uint16_t;
using signed_byte_t = int8_t;
using signed_word_t = int16_t;

const int c_TILE_SIZE    = 16; // Bytes !
const int c_VIDEO_WIDTH  = 160;
const int c_VIDEO_HEIGHT = 144;

const unsigned int c_MEMORY_SIZE            = 0xFFFF;
const unsigned int c_CARTRIDGE_MEMORY_SIZE  = 0x200000;
const unsigned int c_RAM_BANKS_MEMORY_SIZE  = 0x8000;
const unsigned int c_ROM_BANK_SIZE          = 0x4000;
const unsigned int c_RAM_BANK_SIZE          = 0x2000;

const unsigned int c_INSTRUCTION_TABLE_SIZE    = 0xFF;
const unsigned int c_PREFIXED_INSTRUCTION_BYTE = 0xCB;

const int c_MAX_CYCLES_PER_UPDATE = 69905;
const int c_CLOCKSPEED            = 4194304;
const int c_DIVIDER_CYCLE_FREQ    = 256; // Divider register update frequency in cycles

const word_t c_TOP_OF_STACK     = 0xFFFE;
const word_t c_INITIAL_PC_VALUE = 0x100u;
const word_t c_VBLANK_INTERUPT  = 0x40;
const word_t c_LCD_INTERUPT     = 0x48;
const word_t c_TIMER_INTERUPT   = 0x50;
const word_t c_JOYPAD_INTERUPT  = 0x60;

const std::string c_BOOT_ROM_LOCATION = "/home/randa/Documents/emulation/gb_cpp/roms/DMG_ROM.bin";
const int c_BOOT_ROM_SIZE = 0x100;

//Cartridge Header
const word_t r_CARTRIDGE_TYPE  = 0x0147;
const word_t r_CARTRIDGE_TITLE = 0x0134; //16 bytes long

//Registers
const word_t c_INTERUPTS_REQ_ADDRESS     = 0xFF0F;
const word_t c_INTERUPTS_ENABLED_ADDRESS = 0xFFFF;

const word_t r_JOYP   = 0xFF00; // Joypad Register

const word_t r_DIV    = 0xFF04; // Divider Register
const word_t r_TIMA   = 0xFF05; // timer
const word_t r_TMA    = 0xFF06; // timer modulator
const word_t r_TMC    = 0xFF07; // timer controller

const word_t r_LCDC   = 0xFF40; //LCD Control register
const word_t r_STAT   = 0xFF41; //LCD status
const word_t r_SCY    = 0xFF42; //Viewport (top left) Y position 
const word_t r_SCX    = 0xFF43; //Viewport (top left) X position 
const word_t r_LY     = 0xFF44; //LCD Y coordinate, indicates current line
const word_t r_LYC    = 0xFF45; //LY compare, used to check if LYC=LY and set a flag in STAT
const word_t r_DMAT   = 0xFF46; //Initiate Direct Memory Access transfer
const word_t r_BG_PALLET         = 0xFF47; //bg colour pallet
const word_t r_SPRITE_PALLET1    = 0xFF48; //first sprite colour pallet
const word_t r_SPRITE_PALLET2    = 0xFF49; //second sprite colour pallet
const word_t r_WY     = 0xFF4A; //Window (top left) Y position 
const word_t r_WX     = 0xFF4B; //Window (top left) X position plus 7

#endif