
#ifndef H_MEMORYBUS
#define H_MEMORYBUS

#include "consts.h"
#include "BSlogger.h"

class MemoryBus
{
private:
    logger& m_log;

    byte_t m_Memory[c_MEMORY_SIZE]{};
    byte_t m_CartridgeMemory[c_CARTRIDGE_MEMORY_SIZE]{};
    byte_t m_RAMBankMemory[c_RAM_BANKS_MEMORY_SIZE]{};

    byte_t m_CurrentROMBank{1}; //the current rom bank "loaded" into 0x4000-0x7FFF
    byte_t m_CurrentRAMBank{};

    bool m_MBC1{}; //rom banking mode 1
    bool m_MBC2{}; //rom banking mode 2
    bool m_ROMBanking{};
    bool m_EnableRAM{};
    bool m_bootRomLoaded{};

    void loadBootRom();
    void unloadBootRom();
    void getRomBankingMode();
    void updateBanking(word_t address, byte_t value);
    void ramBankEnable(word_t address, byte_t value);
    void changeLoROMBank(byte_t value);
    void changeHiRomBank(byte_t value);
    void RAMBankChange(byte_t value);
    void changeROMRAMMode(byte_t value);

public:
    MemoryBus(logger& logRef);

    byte_t readByte(word_t address) const;
    void writeByte(word_t address, byte_t value);
    void loadGame(const char* file);
    void increment(word_t address);
    std::string getTitle();
};


#endif