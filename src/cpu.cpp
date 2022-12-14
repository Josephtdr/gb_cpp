#include "inc/cpu.h"
#include "inc/bitfuncs.h"

#include <iostream> 
#include <stdexcept>
#include <cassert>

CPU::CPU(MemoryBus& memoryRef, logger& logRef, Platform& platformRef, 
        PPU& ppuRef, APU& apuRef, Settings& settingsRef)
 : m_SP{ c_TOP_OF_STACK },
   m_log{ logRef },
   m_Memory{ memoryRef },
   m_Platform{ platformRef },
   m_PPU{ ppuRef },
   m_InteruptsEnabled{ false },
   m_APU{ apuRef },
   m_Settings{ settingsRef }
   
{
    std::cout << std::hex;
    std::cerr << std::hex;

    m_PC = (settingsRef.bootRom ? 0 : 0x100);

    m_Registers.set_af(0x01B0);
    m_Registers.set_bc(0x0013);
    m_Registers.set_de(0x00D8);
    m_Registers.set_hl(0x014D);

    setupTables();
}

void CPU::logOpcode(word_t PC, byte_t opcode, byte_t arg1, byte_t arg2, std::string_view func, std::string_view peram1, std::string_view peram2) const
{
    if (!m_Settings.traceLog || m_Memory.bootRomLoaded())
        return;

    std::string comma = ((peram1!="" && peram2!="")? ", " : "  ");

    int len = 15 - (func.length() + peram1.length() + peram2.length() + comma.length());

    m_log(LOG_INFO) << std::hex << std::uppercase << std::setfill('0') <<
        std::setw(4) << +PC << ":  " <<
        "0x" << std::setw(2) << +opcode << " " << 
        std::setw(2) << +arg1 << " " <<
        std::setw(2) << +arg2 << "  " << std::setfill(' ') <<
        func << " " << peram1 << comma << peram2 <<
        std::setw(len) << "" << std::setfill('0') << 
        "A:" << std::setw(2) << +m_Registers.a << " " << 
        "F:" << std::setw(2) << +m_Registers.f << " " <<
        "B:" << std::setw(2) << +m_Registers.b << " " <<
        "C:" << std::setw(2) << +m_Registers.c << " " <<
        "D:" << std::setw(2) << +m_Registers.d << " " <<
        "E:" << std::setw(2) << +m_Registers.e << " " <<
        "H:" << std::setw(2) << +m_Registers.h << " " <<
        "L:" << std::setw(2) << +m_Registers.l << "  " <<
        "SP:" << std::setw(4) << +m_SP << "  " <<
        "PCMEM:"<< std::setw(2) << +m_Memory.readByteRaw(PC) << "," <<
                   std::setw(2) << +m_Memory.readByteRaw(PC+1) << "," <<
                   std::setw(2) << +m_Memory.readByteRaw(PC+2) << "," <<
                   std::setw(2) << +m_Memory.readByteRaw(PC+3) <<"\n";
}

int CPU::cycle()
{
    byte_t instructionByte{ readNextByte() };
    
    if (m_haltBug)
    {
        m_haltBug = false;
        --m_PC;
    }

    bool prefixed{ instructionByte == c_PREFIXED_INSTRUCTION_BYTE };
    if (prefixed)
        instructionByte = readNextByte(); 
    
    return execute(instructionByte, prefixed);
}

int CPU::execute(byte_t instructionByte, bool prefixed)
{
    if (!prefixed)
        return opcode_Translator(instructionByte);
    else
        return CBopcode_Translator(instructionByte);
}

bool CPU::isHalted() { return m_Halted; }
bool CPU::isStopped() { return m_Stopped; }

void CPU::update(int cycles)
{
    updateTimers(cycles);
    updateJoypad();

    if (m_IMEScheduled==1)
        ++m_IMEScheduled;
    else if (m_IMEScheduled>1)
    {
        m_IMEScheduled = 0;
        m_InteruptsEnabled = true;
    }
}

byte_t CPU::readByte(word_t address) const
{
    // //Vram Area
    // if ((address>=0x8000u) && (address < 0xA000u))
    // {
    //     int mode{ extractBits(readByte(r_STAT), 0, 2) };
    //     if (mode==3)
    //         return 0xFF;
    //     else
    //         return m_Memory.readByte(address);
    // }
    // // OAM ram
    // else if ((address >= 0xFE00u) && (address <= 0xFE9Fu))
    // {
    //     int mode{ extractBits(readByte(r_STAT), 0, 2) };
    //     if (mode==2||mode==3)
    //         return 0xFF;
    //     else
    //         return m_Memory.readByte(address);
    // }
    // //working ram
    // else if ((address >= 0xC000u) && (address < 0xE000u))
    // {   
    //     auto value{ m_Memory.readByte(address) };
    //     if (address == 0xdd00)
    //         m_log(LOG_ERROR) << "PC: " << +m_PC <<  " Reading From WRAM " <<
    //             "value: " << +value << " address: "<<+address <<"\n";
    //     return value;
    // }  
    
    if (address>=0xFF10 && address < 0xFF40)
    {
        return readAudioRegister(address);
    }
    else
    {
        return m_Memory.readByte(address);
    }    
}

byte_t CPU::readAudioRegister(word_t address) const
{
    static const std::vector<byte_t> orList{
        0x80, 0x3F, 0x00, 0xFF, 0xBF, 
        0xFF, 0x3F, 0x00, 0xFF, 0xBF, 
        0x7F, 0xFF, 0x9F, 0xFF, 0xBF, 
        0xFF, 0xFF, 0x00, 0x00, 0xBF, 
        0x00, 0x00, 0x70,
    };
    //registers
    if (address>=0xFF10&&address<=0xFF26)
    {
        auto orValue {orList[address-0xFF10]};
        return m_Memory.readByte(address) | orValue;
    }
    //unused
    else if (address>=0xFF27 && address <= 0xFF2F)
        return 0xFF;
    //wave ram
    else
    {
        //if channel is active, ignore read
        auto nr52 {m_Memory.readByte(r_NR52)};
        if(testBit(nr52,2))
            return 0xFF;
        else
            m_Memory.readByte(address);
    }
}

void CPU::writeByte(word_t address, byte_t value)
{
    // //Vram Area
    // if ((address>=0x8000u) && (address < 0xA000u))
    // {
    //     int mode{ extractBits(readByte(r_STAT), 0, 2) };
    //     if (mode==3)
    //         return;
    //     else
    //         m_Memory.writeByte(address, value);
    // }
    // // OAM ram
    // else if ((address >= 0xFE00u) && (address <= 0xFE9Fu))
    // {
    //     int mode{ extractBits(readByte(r_STAT), 0, 2) };
    //     if (mode==2||mode==3)
    //         return;
    //     else
    //         m_Memory.writeByte(address, value);
    // }
    // if ((address >= 0xE000u) && (address < 0xFE00u))
    // {
    //     m_log(LOG_ERROR) << "PC: " << +m_PC <<  " WRITING INTO ECHO RAM " <<
    //             "value: " << +value << " address: "<<+address <<"\n";
    //     m_Memory.writeByte(address, value);
    // }
    //  if ((address >= 0xC000u) && (address < 0xE000u))
    // {   
    //     if (address == 0xdd00)
    //         m_log(LOG_ERROR) << "PC: " << +m_PC <<  " writing into WRAM " <<
    //           "value: " << +value << " address: "<<+address <<"\n";
    //     m_Memory.writeByte(address, value);
    // }

    if (address>=0xFF10 && address < 0xFF40)
    {
        writeAudioRegister(address, value);
    }
    //reset the current scanline if the game tries to write to it
    else if (address == r_DIV)
    {
        if (testBit(readByte(r_DIV),4))
            m_APU.incrementDivAPU();
        m_Memory.writeByte(r_DIV, 0);
    }
    else if (address == r_LY)
    {
        m_Memory.writeByte(r_LY, 0);
        m_PPU.m_ScanlineCounter = 456; //number of cycles per scanline
    }
    else if (address == r_DMAT)
    {
        initiateDMATransfer(value);
    }
    else if (address == r_TMC)
    {
        byte_t currentfreq = getClockFreq();
        m_Memory.writeByte(r_TMC, value);
        if (currentfreq != getClockFreq())
            updateClockFreq();
    }
    else
        m_Memory.writeByte(address, value);
}

void CPU::writeAudioRegister(word_t address, byte_t value)
{
    //registers
    if (address==r_NR14)
    {
        //TODO: if channel is triggered, dont alter low two bits of the frequency timer
        m_Memory.writeByte(address, value);
        m_APU.control(0,value);
    }
    else if (address==r_NR24)
    {
        //TODO: if channel is triggered, dont alter low two bits of the frequency timer
        m_Memory.writeByte(address, value);
        m_APU.control(1,value);
    }
    else if (address==r_NR34)
    {
        m_Memory.writeByte(address, value);
        m_APU.control(2,value);
    }
    else if (address==r_NR44)
    {
        m_Memory.writeByte(address, value);
        m_APU.control(3,value);
    }
    else if (address==r_NR52)
    {
        auto nr52 {m_Memory.readByte(r_NR52)};
        nr52 &= 0x7F;

        m_Memory.writeByte(address, (value & 80) | nr52);
        m_APU.toggle(value);
    }
    //unused
    else if (address>=0xFF27 && address<=0xFF2F)
        return;
    //wave ram
    else if (address>=0xFF30 && address<=0xFF3F)
    {
        //if channel is active, ignore write
        auto nr52 {m_Memory.readByte(r_NR52)};
        if(testBit(nr52,2))
            return;
        else
            m_Memory.writeByte(address, value);
    }
    //other
    else
    {
        m_Memory.writeByte(address, value);
    }
}

void CPU::updateTimers(int cycles)
{
    updateDividerRegister(cycles);

    // the clock must be enabled to update the clock
    if (!isClockEnabled())
        return;

    m_TimerCounter -= cycles;
    // enough cpu clock cycles have happened to update the timer
    while (m_TimerCounter <= 0)
    {
        m_log(LOG_DEBUG) << "Timer reset!" << "\n";
        // reset m_TimerTracer to the correct value
        updateClockFreq();

        // timer about to overflow
        if (readByte(r_TIMA) == 255)
        {
            writeByte(r_TIMA,readByte(r_TMA)) ;
            requestInterupt(2);
        }
        else
            m_Memory.increment(r_TIMA);
    }
}

bool CPU::isClockEnabled() const
{
    return testBit(readByte(r_TMC), 2);
}
byte_t CPU::getClockFreq() const
{
    return readByte(r_TMC) & 0b11u;
}
void CPU::updateClockFreq()
{
    byte_t freq = getClockFreq();
    switch (freq)
    {
        case 0: m_TimerCounter += 1024; break; // freq 4096
        case 1: m_TimerCounter += 16; break;// freq 262144
        case 2: m_TimerCounter += 64; break;// freq 65536
        case 3: m_TimerCounter += 256; break;// freq 16382
    }
}

/**
 * @brief APU div counter incremented every time bit 4 goes from 1-0
 * 
 * @param cycles 
 */
void CPU::updateDividerRegister(int cycles)
{
    m_DividerCounter+=cycles;
    while (m_DividerCounter >= c_DIVIDER_CYCLE_FREQ)
    {
        bool bit4Set {testBit(readByte(r_DIV),4)};

        m_DividerCounter -= c_DIVIDER_CYCLE_FREQ;
        m_Memory.increment(r_DIV);

        if (bit4Set && !testBit(readByte(r_DIV),4))
            m_APU.incrementDivAPU();
    }
}

void CPU::updateJoypad()
{
    const auto [type, key] {m_Platform.ProcessInput()};

    switch(type)
    {
        case 0: keyDown(key); break;
        case 1: keyUp(key); break;
        case 2: break;
    }
}

void CPU::keyDown(int key)
{
    byte_t joyp{ readByte(r_JOYP) };
    bool wasUnpressed{ testBit(m_Memory.m_Joypad, key) };

    resetBit(m_Memory.m_Joypad, key); 

    bool isButton{ key > 3 };
    bool interupt{};

    if (isButton && !testBit(joyp,5))
        interupt = true;
    else if (!isButton && !testBit(joyp,4))
        interupt = true;

    if (interupt && wasUnpressed)
        requestInterupt(4);
}
void CPU::keyUp(int key)
{
    setBit(m_Memory.m_Joypad, key);
}

/**
 * @brief checks and executes any valid requested interupts in the priority 
 * 0 (V-BLANK), 1 (LCD), 2 (TIMER), 3 (SERIAL), 4 (JOYPAD).
 * @return 20 cycles if an interupt is executed, else 0;  
 */
int CPU::interupts()
{
    byte_t requests = readByte(r_IF)&0x1F;
    byte_t enabled = readByte(r_IE)&0x1F;

    if (!m_InteruptsEnabled) //master switch disabled
    {
        //cpu halted and there is a pending, enabled, interupt
        if (m_Halted && (requests&enabled))
            m_Halted = false;

        return 0;
    }
    if (!requests) //no requests pending
        return 0;

    //for each interupt bit 0-4
    for (byte_t i=0; i<5; ++i) 
    {
        //There is a request (i) and it is enabled 
        if (testBit(requests,i) && testBit(enabled,i)) 
        {
            performInterupt(i);
            return 20;
        }
    }

    return 0;
}


/**
 * @brief 0 Vblank, 1 LCD, 2 Timer, 4 Joypad input
 */
void CPU::requestInterupt(int interupt) //0,1,2,4
{
    assert((interupt>=0&&interupt<5) && "Invalid interupt requested!");
    byte_t requests = readByte(r_IF);
    
    setBit(requests, interupt);
    writeByte(r_IF, requests);
}

/**
 * @brief Adds the current pc to the stack, and sets it to the 
 * assosated interupt start address dictated by the param
 * @param interupt 
 */
void CPU::performInterupt(int interupt)
{
    m_log(LOG_DEBUG) << "Starting interupt " << interupt << "!" << "\n";
    m_InteruptsEnabled = false;    
    m_Halted = false;
    if (interupt==4) //joypad interupt
        m_Stopped = false;

    byte_t requests = readByte(r_IF);
    resetBit(requests, interupt);
    writeByte(r_IF, requests);

    push(m_PC);
    switch(interupt)
    {
        case 0: m_PC = c_VBLANK_INTERUPT; break;
        case 1: m_PC = c_LCD_INTERUPT; break;
        case 2: m_PC = c_TIMER_INTERUPT; break;
        case 3: m_PC = c_SERIAL_INTERUPT; break;
        case 4: m_PC = c_JOYPAD_INTERUPT; break;
        default: 
            throw std::logic_error("Invalid interupt code!");
    }
}

/**
 * @brief Performs the Direct Memory Access transfer, which maps the sprite data 
 * at address 'value' * 100, to the sprite ram at (0xFE00-0xFE9F).
 */
void CPU::initiateDMATransfer(byte_t value)
{
    word_t startAddress{ static_cast<word_t>(value << 8) };
    for (int i{0}; i<0xA0; i++)
    {
        writeByte(0xFE00+i, readByte(startAddress+i));
    }
}