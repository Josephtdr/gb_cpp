#ifndef H_CONSTS
#define H_CONSTS

#include <cstdint>
#include <string>
#include <vector>

using byte_t = uint8_t;
using word_t = uint16_t;
using signed_byte_t = int8_t;
using signed_word_t = int16_t;

constexpr int c_TILE_SIZE    = 16; // Bytes !
constexpr int c_VIDEO_WIDTH  = 160;
constexpr int c_VIDEO_HEIGHT = 144;

constexpr unsigned int c_MEMORY_SIZE            = 0x10000;
constexpr unsigned int c_CARTRIDGE_MEMORY_SIZE  = 0x200000;
constexpr unsigned int c_RAM_BANKS_MEMORY_SIZE  = 0x8000;
constexpr unsigned int c_ROM_BANK_SIZE          = 0x4000;
constexpr unsigned int c_RAM_BANK_SIZE          = 0x2000;

constexpr unsigned int c_INSTRUCTION_TABLE_SIZE    = 0x100;
constexpr unsigned int c_PREFIXED_INSTRUCTION_BYTE = 0xCB;

constexpr int c_MAX_CYCLES_PER_UPDATE = 69905;
constexpr int c_CLOCKSPEED            = 4194304;
constexpr int c_DIVIDER_CYCLE_FREQ    = 256; // Divider register update frequency in cycles

constexpr word_t c_TOP_OF_STACK     = 0xFFFE;
constexpr word_t c_INITIAL_PC_VALUE = 0x100u;
constexpr word_t c_VBLANK_INTERUPT  = 0x40;
constexpr word_t c_LCD_INTERUPT     = 0x48;
constexpr word_t c_TIMER_INTERUPT   = 0x50;
constexpr word_t c_SERIAL_INTERUPT  = 0x58;
constexpr word_t c_JOYPAD_INTERUPT  = 0x60;

const     std::string c_BOOT_ROM_LOCATION = "/home/randa/Documents/emulation/gb_cpp/roms/DMG_ROM.bin";
constexpr int c_BOOT_ROM_SIZE = 0x100;

//Cartridge Header
constexpr word_t r_CARTRIDGE_TYPE  = 0x0147;
constexpr word_t r_CARTRIDGE_TITLE = 0x0134; //16 bytes long

//Registers
constexpr word_t r_IF     = 0xFF0F; // interrupt flags (request register)
constexpr word_t r_IE     = 0xFFFF; // interupt enabled register

constexpr word_t r_JOYP   = 0xFF00; // Joypad Register

constexpr word_t r_DIV    = 0xFF04; // Divider Register
constexpr word_t r_TIMA   = 0xFF05; // timer
constexpr word_t r_TMA    = 0xFF06; // timer modulator
constexpr word_t r_TMC    = 0xFF07; // timer controller

constexpr word_t r_LCDC   = 0xFF40; //LCD Control register
constexpr word_t r_STAT   = 0xFF41; //LCD status
constexpr word_t r_SCY    = 0xFF42; //Viewport (top left) Y position 
constexpr word_t r_SCX    = 0xFF43; //Viewport (top left) X position 
constexpr word_t r_LY     = 0xFF44; //LCD Y coordinate, indicates current line
constexpr word_t r_LYC    = 0xFF45; //LY compare, used to check if LYC=LY and set a flag in STAT
constexpr word_t r_DMAT   = 0xFF46; //Initiate Direct Memory Access transfer
constexpr word_t r_BG_PALLET         = 0xFF47; //bg colour pallet
constexpr word_t r_SPRITE_PALLET1    = 0xFF48; //first sprite colour pallet
constexpr word_t r_SPRITE_PALLET2    = 0xFF49; //second sprite colour pallet
constexpr word_t r_WY     = 0xFF4A; //Window (top left) Y position 
constexpr word_t r_WX     = 0xFF4B; //Window (top left) X position plus 7

//APU
constexpr word_t r_NR52 = 0xFF26; //sound on off (bit 7 global, 0-3=ch 1-4)
constexpr word_t r_NR51 = 0xFF25; //sound panning
constexpr word_t r_NR50 = 0xFF24; //master volume 1-8, vin panning
//sweep+pulse
constexpr word_t r_NR10 = 0xFF10; //sweep
constexpr word_t r_NR11 = 0xFF11; //length timer & duty cycle
constexpr word_t r_NR12 = 0xFF12; //volume & envelope
constexpr word_t r_NR13 = 0xFF13; //wavelength low 
constexpr word_t r_NR14 = 0xFF14; //wavelength high (0-2) & control
//pulse
constexpr word_t r_NR21 = 0xFF16;
constexpr word_t r_NR22 = 0xFF17;
constexpr word_t r_NR23 = 0xFF18;
constexpr word_t r_NR24 = 0xFF19;
//wave
constexpr word_t r_NR30 = 0xFF1A; //dac enable
//TODO: m_memory fucntion, write to here enables disables this dac + channel
constexpr word_t r_NR31 = 0xFF1B; //length timer
constexpr word_t r_NR32 = 0xFF1C; //volume 
constexpr word_t r_NR33 = 0xFF1D; //wavelength low 
constexpr word_t r_NR34 = 0xFF1E; //wavelength high (0-2) & control
//noise
constexpr word_t r_NR41 = 0xFF20; //length timer
constexpr word_t r_NR42 = 0xFF21; //volume & envelope
constexpr word_t r_NR43 = 0xFF22; //frequency & randomness 
constexpr word_t r_NR44 = 0xFF23; //control

const std::vector<word_t> r_UNMAPPED
{
    0xFF08, 0xFF4D, 0xFF56, 0xFF4F,0xFF51,0xFF52,0xFF53,0xFF54,0xFF55,  
    0xFF68,0xFF69,0xFF6A,0xFF6B,0xFF6C,0xFF70,0xFF72,0xFF73,0xFF74,0xFF75,0xFF76,0xFF77
};

struct Settings {
  bool traceLog {false};
  bool bootRom {false};
  bool disableAudio{false};
};

#endif