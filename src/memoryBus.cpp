#include "inc/memoryBus.h"
#include "inc/bitfuncs.h"

#include <stdexcept> // for std::runtime_error
#include <cstring> //memset, 
#include <fstream> //streams
#include <iostream>
#include <algorithm>
#include <string>

MemoryBus::MemoryBus(logger& logRef)
    : m_log{ logRef },
      m_Joypad{ 0xFF },
      m_CurrentROMBank{ 1 }
{
    m_Memory[0xFF00] = 0xFFu; //   ; JOYP
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

byte_t MemoryBus::getJoypadState() const
{
    byte_t joyp{ m_Memory[r_JOYP] };

    if (!testBit(joyp, 4)) //regular buttons
    {
            byte_t topJoypad{ static_cast<byte_t>(m_Joypad & 0xF) };
            topJoypad |= 0xF0; // turn the top 4 bits on
            joyp &= topJoypad; // show what buttons are pressed
    }
    else if (!testBit(joyp,5))//directional buttons
    {
            byte_t bottomJoypad{ static_cast<byte_t>(m_Joypad >> 4) };
            bottomJoypad |= 0xF0;
            joyp &= bottomJoypad;
    }
    m_log(LOG_DEBUG) << "Reading joypad state as: " << +joyp << ", currently r_JOYP: " << +m_Memory[r_JOYP] <<"\n";
    return joyp ;
}

byte_t MemoryBus::readByteRaw(word_t address)
{
    if (address < 0x4000u) 
    {
        //boot rom currently loaded
        if (m_bootRomLoaded && (address < 0x100))
            return m_Memory[address];
        
        return m_CartridgeMemory[address];
    }
    //rom banking area so access cartridge memory instead
    else if ((address>=0x4000u) && (address < 0x8000u))
    {   
        return readRomBank(address);
    }
    // ram memory bank area
    else if ((address >= 0xA000u) && (address < 0xC000u))
    {
        word_t newAddress{ static_cast<word_t>(address - 0xA000) };
        newAddress += (m_CurrentRAMBank*c_RAM_BANK_SIZE);
        return m_RAMBankMemory[newAddress];
    }
    //restricted
    else if (((address >= 0xFEA0u) && (address <= 0xFEFFu) )|| 
        std::find(r_UNMAPPED.begin(),r_UNMAPPED.end(),address) != r_UNMAPPED.end() )
    {
        return 0xFF;
    }
    else
    {
        return m_Memory[address];
    }
}

byte_t MemoryBus::readByte(word_t address)
{
    if (address < 0x4000u) 
    {
        //boot rom currently loaded
        if (m_bootRomLoaded && (address < 0x100))
        {
            return m_Memory[address];
        }
        return m_CartridgeMemory[address];
    }
    //rom banking area so access cartridge memory instead
    else if ((address>=0x4000u) && (address <= 0x7FFFu))
    {   
        return readRomBank(address);
    }
    //Vram Area
    else if ((address>=0x8000u) && (address < 0xA000u))
    {
        return m_Memory[address];
    }
    // ram memory bank area
    else if ((address >= 0xA000u) && (address < 0xC000u))
    {
        word_t newAddress{ static_cast<word_t>(address - 0xA000) };
        newAddress += (m_CurrentRAMBank*c_RAM_BANK_SIZE);
        return m_RAMBankMemory[newAddress];
    }
    //read from echo ram
    else if ((address >= 0xE000u) && (address < 0xFE00u))
    {
        readByte(address-0x2000);
    }
    //OAM attribute table
    else if ((address >= 0xFE00u) && (address < 0xFEA0u))
    {
        return m_Memory[address];
    }
    // restricted memory area
    else if (((address >= 0xFEA0u) && (address <= 0xFEFFu) )|| 
        std::find(r_UNMAPPED.begin(),r_UNMAPPED.end(),address) != r_UNMAPPED.end() )
    {
        return 0xFF;
    }
    else if (address == r_JOYP)
    {
        return getJoypadState();
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
    //Vram Area
    else if ((address>=0x8000u) && (address < 0xA000u))
    {
        m_Memory[address] = value;
    }
    else if ((address >= 0xA000) && (address < 0xC000))
    {
        if (m_EnableRAM)
        {
            word_t newAddress{ static_cast<word_t>(address-0xA000) };
            newAddress += (m_CurrentRAMBank*c_RAM_BANK_SIZE);
            m_RAMBankMemory[newAddress] = value ;
        }
    }
    //echo from echo ram
    else if ((address >= 0xE000u) && (address < 0xFE00u))
    {
        writeByte(address-0x2000, value);
    }
    // OAM ram
    else if ((address >= 0xFE00u) && (address < 0xFEA0u))
    {
        m_Memory[address] = value;
    }
    //restricted memory area
    else if ( ((address >= 0xFEA0u) && (address <= 0xFEFFu) )|| 
        std::find(r_UNMAPPED.begin(),r_UNMAPPED.end(),address) != r_UNMAPPED.end() )
    {
        return;
    }
    //joypad write
    else if (address == r_JOYP)
    {
        m_Memory[r_JOYP] = (value | 0x0F);
    }
    else if (address == r_DIV)
    {
        m_Memory[r_DIV] = 0;
    }
    else if ((address == 0xFF50) && value) //unmap  bootrom
    {
        unloadBootRom();
    }
    else
    {
        m_Memory[address] = value;
    }
}

void MemoryBus::increment(word_t address)
{
    ++m_Memory[address];
}

void MemoryBus::loadGame(const char* filename, bool bootRom)
{
    std::ifstream gameRom(filename, std::ifstream::binary | std::ios::ate);
    int fileLen = gameRom.tellg();
    gameRom.seekg (0, std::ios::beg);
    gameRom.read(reinterpret_cast<char*>(m_CartridgeMemory.data()), fileLen);


    m_log(LOG_DEBUG) << "Loaded game into game rom!" << "\n";
    getRomBankingMode();
    if (bootRom)
        loadBootRom();
}

std::string MemoryBus::getTitle()
{
    std::vector<byte_t> buffer{};

    for (byte_t i{}; i<16; ++i)
    {
        byte_t& character = m_CartridgeMemory[r_CARTRIDGE_TITLE + i]; 
        if (character==0) break;

        buffer.push_back(character);
    }
    return std::string {buffer.begin(), buffer.end()};
}

void MemoryBus::loadBootRom()
{
    char buffer[c_BOOT_ROM_SIZE]{};
    std::ifstream bootRom(c_BOOT_ROM_LOCATION.c_str(), std::ifstream::binary);
    bootRom.read(buffer, c_BOOT_ROM_SIZE);

    for (int i = 0; i < c_BOOT_ROM_SIZE; ++i)
    {
        m_Memory[i] = buffer[i];
    }

    m_bootRomLoaded = true;
    m_log(LOG_DEBUG) << "Loaded boot rom into memory!" << "\n";
    m_log.set_log_level(LOG_INFO);
}

void MemoryBus::unloadBootRom()
{
    if (m_bootRomLoaded)
    {
        m_log(LOG_DEBUG) << "Unmapping boot rom!" << "\n" << "\n";
        m_bootRomLoaded = false;
    }
}

bool MemoryBus::bootRomLoaded() const
{
    return m_bootRomLoaded;
}

byte_t MemoryBus::readRomBank(word_t address)
{
    if (!m_MBC1 && !m_MBC2)
        return m_CartridgeMemory[address];

    if (m_CurrentROMBank == 0) //KEEPS GETTING SET TO 0??????
        m_CurrentROMBank = 1;
        
    unsigned int newAddress{ static_cast<word_t>(address - c_ROM_BANK_SIZE) };
    newAddress += (m_CurrentROMBank*c_ROM_BANK_SIZE);
    return m_CartridgeMemory[newAddress];
}

void MemoryBus::getRomBankingMode()
{
    switch (m_CartridgeMemory[r_CARTRIDGE_TYPE])   
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
        default: break;
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
        if (m_CurrentROMBank == 0) 
            m_CurrentROMBank = 1; //cant be zero
        return;
    }
    
    byte_t lower5 = value & 0b00011111u ; //save lower 5
    m_CurrentROMBank &= 0b11100000u; // turn off the lower 5
    m_CurrentROMBank |= lower5 ;
    if (m_CurrentROMBank == 0) 
        m_CurrentROMBank = 1;
    m_log(LOG_DEBUG) << "m_CurrentROMBank is now" << std::dec << m_CurrentROMBank << std::hex << "\n";
}

void MemoryBus::changeHiRomBank(byte_t value)
{
    // turn off the upper 3 bits of the current rom
    m_CurrentROMBank &= 0b00011111u ;

    // turn off the lower 5 bits of the data
    value &= 0b11100000u ;
    m_CurrentROMBank |= value ;
    if (m_CurrentROMBank == 0)
        m_CurrentROMBank = 1; //cant be zero
    m_log(LOG_DEBUG) << "m_CurrentROMBank is now" << std::dec << m_CurrentROMBank << std::hex << "\n";
}

void MemoryBus::RAMBankChange(byte_t value)
{
    m_CurrentRAMBank = value & 0x3u;
    m_log(LOG_DEBUG) << "m_CurrentRAMBank is now" << std::dec << m_CurrentRAMBank << std::hex << "\n";
}
void MemoryBus::changeROMRAMMode(byte_t value)
{
    word_t newData = value & 1u ;
    m_ROMBanking = !newData;
    if (m_ROMBanking)
        m_CurrentRAMBank = 0;

    m_log(LOG_DEBUG) << "m_CurrentRAMBank is now" << std::dec << m_CurrentRAMBank << std::hex << "\n";
    
}