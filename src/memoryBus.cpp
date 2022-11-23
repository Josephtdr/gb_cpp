#include "memoryBus.h"

#include <stdexcept> // for std::runtime_error
#include<cstring> //memset, 
#include <fstream> //streams

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

    word_t testData = value & 0xF;
    if (testData == 0xA)
        m_EnableRAM = true;
    else if (testData == 0x0)
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
    word_t newData = value & 0x1u ;
    m_ROMBanking = newData == 0;
    if (m_ROMBanking)
        m_CurrentRAMBank = 0 ;
}