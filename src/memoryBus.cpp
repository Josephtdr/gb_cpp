#include "memoryBus.h"

#include <stdexcept> // for std::runtime_error
#include<cstring> //memset, 
#include <fstream> //streams

MemoryBus::MemoryBus(int& timerRef)
    : m_timerCounterRef{ timerRef }
{
    m_Memory[0xFF05] = 0x00u; //   ; TIMA
    m_Memory[0xFF06] = 0x00u; //   ; TMA
    m_Memory[0xFF07] = 0x00u; //   ; TAC
    m_Memory[0xFF10] = 0x80u; //   ; NR10
    m_Memory[0xFF11] = 0xBFu; //   ; NR11
    m_Memory[0xFF12] = 0xF3u; //   ; NR12
    m_Memory[0xFF14] = 0xBFu; //   ; NR14
    m_Memory[0xFF16] = 0x3Fu; //   ; NR21
    m_Memory[0xFF17] = 0x00u; //   ; NR22
    m_Memory[0xFF19] = 0xBFu; //   ; NR24
    m_Memory[0xFF1A] = 0x7Fu; //   ; NR30
    m_Memory[0xFF1B] = 0xFFu; //   ; NR31
    m_Memory[0xFF1C] = 0x9Fu; //   ; NR32
    m_Memory[0xFF1E] = 0xBFu; //   ; NR33
    m_Memory[0xFF20] = 0xFFu; //   ; NR41
    m_Memory[0xFF21] = 0x00u; //   ; NR42
    m_Memory[0xFF22] = 0x00u; //   ; NR43
    m_Memory[0xFF23] = 0xBFu; //   ; NR30
    m_Memory[0xFF24] = 0x77u; //   ; NR50
    m_Memory[0xFF25] = 0xF3u; //   ; NR51
    m_Memory[0xFF26] = 0xF1u; //   ; NR52
    m_Memory[0xFF40] = 0x91u; //   ; LCDC
    m_Memory[0xFF42] = 0x00u; //   ; SCY
    m_Memory[0xFF43] = 0x00u; //   ; SCX
    m_Memory[0xFF45] = 0x00u; //   ; LYC
    m_Memory[0xFF47] = 0xFCu; //   ; BGP
    m_Memory[0xFF48] = 0xFFu; //   ; OBP0
    m_Memory[0xFF49] = 0xFFu; //   ; OBP1
    m_Memory[0xFF4A] = 0x00u; //   ; WY
    m_Memory[0xFF4B] = 0x00u; //   ; WX
    m_Memory[0xFFFF] = 0x00u; //   ; IE
}

byte_t MemoryBus::readByte(word_t address) const
{
    // //initial rom bank
    // if ((address>=0) && (address <= 0x4000)) 
    // {

    // }
    //rom banking area so access cartridge memory instead
    if ((address>=0x4000) && (address <= 0x7FFF))
    {
        word_t newAddress = address - c_ROM_BANK_SIZE;
        return m_CartridgeMemory[newAddress + (m_CurrentROMBank*c_ROM_BANK_SIZE)];
    }
    // are we reading from ram memory bank?
    else if ((address >= 0xA000) && (address <= 0xBFFF))
    {
        word_t newAddress = address - 0xA000;
        return m_RAMBankMemory[newAddress + (m_CurrentRAMBank*c_RAM_BANK_SIZE)];
    }
    //restricted memory area
    else if (0xFEA0u <= address || address <= 0xFEFFu)
    {
        return 0u;
    }
    else
    {
        return m_Memory[address];
    }
}

//TODO: magic numbers
void MemoryBus::writeByte(word_t address, byte_t value)
{
    //read only memory for storing rom data
    if (address < 0x8000u)
    { 
        updateBanking(address, value);
    }
    else if ((address >= 0xA000) && (address < 0xC000))
    {
        if (m_EnableRAM)
        {
            word_t newAddress = address - 0xA000 ;
            m_RAMBankMemory[newAddress + (m_CurrentRAMBank*c_RAM_BANK_SIZE)] = value ;
        }
    }
    //maybe TODO: echo into echo ram?
    //echo from echo ram
    else if (0xE000u <= address || address <= 0xFE00u)
    {
        m_Memory[address] = value;
        writeByte(address-0x2000, value);
    }
    //restricted memory area
    else if (0xFEA0u <= address || address <= 0xFEFFu)
    {
        return;
    }
    //harcoded and hacky because of oop setup
    else if (address == c_TMC_ADDRESS)
    {
        byte_t currentfreq = m_Memory[c_TMC_ADDRESS] & 0b11u;
        m_Memory[c_TMC_ADDRESS] = value;
        byte_t newfreq = m_Memory[c_TMC_ADDRESS] & 0b11u;

        if (currentfreq != newfreq)
        {
            switch (newfreq)
            {
                case 0: m_timerCounterRef = 1024; break; // freq 4096
                case 1: m_timerCounterRef = 16; break;// freq 262144
                case 2: m_timerCounterRef = 64; break;// freq 65536
                case 3: m_timerCounterRef = 256; break;// freq 16382
            }
        }
    }
    else if (c_DIV_REGISTER_ADDRESS == address)
    {
        m_Memory[c_DIV_REGISTER_ADDRESS] = 0 ;
    }
    else
    {
        m_Memory[address] = value;
    }
}


void MemoryBus::loadGame(const char* filename)
{
    memset(m_CartridgeMemory,0,sizeof(m_CartridgeMemory)) ;

    FILE *in;
    in = fopen( filename, "rb" );
    fread(m_CartridgeMemory, 1, 0x200000, in);
    fclose(in); 

    getRomBankingMode();
}

void MemoryBus::getRomBankingMode()
{
    switch (m_CartridgeMemory[0x147])
    {
        case 1:
            m_MBC1 = true; break;
        case 2:
            m_MBC1 = true; break;
        case 3:
            m_MBC1 = true; break;
        case 5:
            m_MBC2 = true; break;
        case 6:
            m_MBC2 = true; break;
        default : break ;
    } 
}

void MemoryBus::updateBanking(word_t address, byte_t value)
{
    if (address < 0x2000)
    {
        if (m_MBC1 || m_MBC2)
        {
            ramBankEnable(address,value) ;
        }
    }
    else if ((address >= 0x2000) && (address < 0x4000))
    {
        if (m_MBC1 || m_MBC2)
        {
            changeLoROMBank(value);
        }
    }
    // do ROM or RAM bank change
    else if ((address >= 0x4000) && (address < 0x6000))
    {
        // there is no rambank in mbc2 so always use rambank 0
        if (m_MBC1)
        {
            if(m_ROMBanking)
            {
                changeHiRomBank(value) ;
            }
            else
            {
                RAMBankChange(value) ;
            }
        }
    }
    else if ((address >= 0x6000) && (address < 0x8000))
    {
        if (m_MBC1)
        {
            changeROMRAMMode(value); 
        }
    }
}

void MemoryBus::ramBankEnable(word_t address, byte_t value)
{
    if (m_MBC2)
    {
        if (address & (1<<4)) return;
    }

    word_t testData = value & 0xFu;
    if (testData == 0xAu)
        m_EnableRAM = true;
    else if (testData == 0u)
        m_EnableRAM = false;
}

void MemoryBus::changeLoROMBank(byte_t value)
{
    if (m_MBC2)
    {
        m_CurrentROMBank = value & 0xF; //set to lower nibble
        if (m_CurrentROMBank == 0) m_CurrentROMBank++; //cant be zero
        return;
    }
    
    byte_t lower5 = value & 0b00011111u ; //save lower 5
    m_CurrentROMBank &= 0b11100000u; // turn off the lower 5
    m_CurrentROMBank |= lower5 ;
    if (m_CurrentROMBank == 0) m_CurrentROMBank++ ;
}

void MemoryBus::changeHiRomBank(byte_t value)
{
    // turn off the upper 3 bits of the current rom
    m_CurrentROMBank &= 0b00011111u ;

    // turn off the lower 5 bits of the data
    value &= 0b11100000u ;
    m_CurrentROMBank |= value ;
    if (m_CurrentROMBank == 0) m_CurrentROMBank++; //cant be zero
}

void MemoryBus::RAMBankChange(byte_t value)
{
    m_CurrentRAMBank = value & 0x3u;
}
void MemoryBus::changeROMRAMMode(byte_t value)
{
    word_t newData = value & 1u ;
    m_ROMBanking = !newData;
    if (m_ROMBanking)
        m_CurrentRAMBank = 0 ;
}

void MemoryBus::incrementDivRegister()
{
    ++m_Memory[c_DIV_REGISTER_ADDRESS];
}