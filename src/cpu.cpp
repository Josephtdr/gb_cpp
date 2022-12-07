#include "inc/cpu.h"
#include "inc/bitfuncs.h"

#include <iostream> 
#include <stdexcept>

CPU::CPU(MemoryBus& memoryRef, logger& logRef, Platform& platformRef, PPU& ppuRef, bool traceLog)
 : m_PC{ 0 },
   m_SP{ c_TOP_OF_STACK },
   m_log{ logRef },
   m_Memory{ memoryRef },
   m_Platform{ platformRef },
   m_PPU{ ppuRef },
   m_InteruptsEnabled{ false },
   m_traceLog{ traceLog }
   
{
    std::cout << std::hex;
    std::cerr << std::hex;

    m_Registers.set_af(0x01B0);
    m_Registers.set_bc(0x0013);
    m_Registers.set_de(0x00D8);
    m_Registers.set_hl(0x014D);

    setupTables();
}

void CPU::logOpcode(word_t PC, byte_t opcode, byte_t arg1, byte_t arg2, std::string_view func, std::string_view peram1, std::string_view peram2) const
{
    if (!m_traceLog || m_Memory.bootRomLoaded())
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
        "PCMEM:"<< std::setw(2) << +m_Memory.readMemForDoctor(PC) << "," <<
                   std::setw(2) << +m_Memory.readMemForDoctor(PC+1) << "," <<
                   std::setw(2) << +m_Memory.readMemForDoctor(PC+2) << "," <<
                   std::setw(2) << +m_Memory.readMemForDoctor(PC+3) <<"\n";
}

int CPU::cycle()
{
    byte_t instructionByte{ readNextByte() };
    bool prefixed{ instructionByte == c_PREFIXED_INSTRUCTION_BYTE };
    if (prefixed)
    { 
        instructionByte = readNextByte(); 
    }
    int nCycles{ execute(instructionByte, prefixed) };

    return nCycles;
}

int CPU::execute(byte_t instructionByte, bool prefixed)
{
    try 
    {
        if (!prefixed)
            { return opcode_Translator(instructionByte); }
        else
            { return CBopcode_Translator(instructionByte); }
    }
    catch (std::runtime_error e)
    {
        m_log(LOG_ERROR) << e.what() << "\n";
        std::exit(EXIT_FAILURE);
    }
}

bool CPU::isHalted() { return m_Halted; }

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
    if (false)//address==r_LY)
    {
        return 0x90;
    }
    // //working ram
    // else if ((address >= 0xC000u) && (address < 0xE000u))
    // {   
    //     auto value{ m_Memory.readByte(address) };
    //     if (address == 0xdd00)
    //         m_log(LOG_ERROR) << "PC: " << +m_PC <<  " Reading From WRAM " <<
    //             "value: " << +value << " address: "<<+address <<"\n";
    //     return value;
    // }  
    else
    {
        return m_Memory.readByte(address);
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
    //reset the current scanline if the game tries to write to it
    if (address == r_LY)
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

void CPU::updateTimers(int cycles)
{
    updateDividerRegister(cycles);

    // the clock must be enabled to update the clock
    if (isClockEnabled())
    {
        m_TimerCounter -= cycles;
        // enough cpu clock cycles have happened to update the timer
        if (m_TimerCounter <= 0)
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
            {
                writeByte(r_TIMA,readByte(r_TIMA)+1) ;
            }
        }
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
        case 0: m_TimerCounter = 1024; break; // freq 4096
        case 1: m_TimerCounter = 16; break;// freq 262144
        case 2: m_TimerCounter = 64; break;// freq 65536
        case 3: m_TimerCounter = 256; break;// freq 16382
    }
}

void CPU::updateDividerRegister(int cycles)
{
    m_DividerCounter+=cycles;
    if (m_DividerCounter >= c_DIVIDER_CYCLE_FREQ)
    {
        m_DividerCounter = 0 ;
        m_Memory.increment(r_DIV);
    }
}

void CPU::updateJoypad()
{
    auto action{ m_Platform.ProcessInput() };

    switch(action.first)
    {
        case 0: keyDown(action.second); break;
        case 1: keyUp(action.second); break;
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
 * 0 (V-BLANK), 1 (LCD), 2 (TIMER), 3 (JOYPAD).
 */
void CPU::interupts()
{
    if (m_InteruptsEnabled)
    {
        byte_t requests = readByte(c_INTERUPTS_REQ_ADDRESS);
        byte_t enabled = readByte(c_INTERUPTS_ENABLED_ADDRESS);
        if (requests)
        {
            for (byte_t i=0; i<5; ++i) //for each interupt bit 0/1/2/4
            {
                if (testBit(requests,i)) //There is a request (i)
                {
                    if (testBit(enabled,i)) //and interupt (i) is enabled 
                    {
                        m_Halted = false;
                        performInterupt(i);
                        return;
                    }
                }
            }
        }
    }
    else if (m_Halted)
    {
        byte_t requests = readByte(c_INTERUPTS_REQ_ADDRESS);
        byte_t enabled = readByte(c_INTERUPTS_ENABLED_ADDRESS);

        if (requests & enabled)
            m_Halted = false;
    }
}


/**
 * @brief 0 Vblank, 1 LCD, 2 Timer, 4 Joypad input
 */
void CPU::requestInterupt(int interupt) //0,1,2,4
{
    byte_t requests = readByte(c_INTERUPTS_REQ_ADDRESS);
    //set relevent bit
    setBit(requests, interupt);
    writeByte(c_INTERUPTS_REQ_ADDRESS, requests);
}

/**
 * @brief Adds the current pc to the stack, and sets it to the 
 * assosated interupt start address dictated by the param
 * @param interupt 
 */
void CPU::performInterupt(int interupt)
{
    m_log(LOG_ERROR) << "Starting interupt " << interupt << "!" << "\n";
    m_InteruptsEnabled = false;

    // m_log.set_log_level(LOG_DEBUG);

    byte_t requests = readByte(c_INTERUPTS_REQ_ADDRESS);
    resetBit(requests, interupt);
    writeByte(c_INTERUPTS_REQ_ADDRESS, requests);

    push(m_PC);
    switch(interupt)
    {
        case 0: m_PC = c_VBLANK_INTERUPT; break;
        case 1: m_PC = c_LCD_INTERUPT; break;
        case 2: m_PC = c_TIMER_INTERUPT; break;
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