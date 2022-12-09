#ifndef H_MEMORYBUS
#define H_MEMORYBUS

#include "consts.h"
#include "BSlogger.h"
#include <array>

class MemoryBus
{
private:
    logger& m_log;

    std::array<byte_t, c_CARTRIDGE_MEMORY_SIZE> m_CartridgeMemory{};
    std::array<byte_t, c_RAM_BANKS_MEMORY_SIZE> m_RAMBankMemory{};
    std::array<byte_t, c_MEMORY_SIZE>           m_Memory{};
    byte_t m_CurrentROMBank{1}; //the current rom bank "loaded" into 0x4000-0x7FFF
    byte_t m_CurrentRAMBank{};

    bool m_MBC1{}; //memory banking mode 1
    bool m_MBC2{}; //memory banking mode 2
    bool m_ROMBanking{};
    bool m_EnableRAM{};
    bool m_bootRomLoaded{};

    void loadBootRom();
    void unloadBootRom();

    byte_t readRomBank(word_t address);
    void getRomBankingMode();
    void updateBanking(word_t address, byte_t value);
    void ramBankEnable(word_t address, byte_t value);
    void changeLoROMBank(byte_t value);
    void changeHiRomBank(byte_t value);
    void RAMBankChange(byte_t value);
    void changeROMRAMMode(byte_t value);
    byte_t getJoypadState() const; 

public:
    MemoryBus(logger& logRef);
    byte_t m_Joypad{};
    
    byte_t readByte(word_t address);
    byte_t readByteRaw(word_t address);
    void writeByte(word_t address, byte_t value);
    void loadGame(const char* file, bool bootRom);
    void increment(word_t address);
    std::string getTitle();
    bool bootRomLoaded() const;
};


#endif