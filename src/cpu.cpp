#include "inc/cpu.h"
#include "inc/bitfuncs.h"

#include <iostream> 
#include <stdexcept>

CPU::CPU(MemoryBus& memoryRef, logger& logRef, Platform& platformRef)
 : m_PC{0},
   m_SP{ c_TOP_OF_STACK },
   m_log{ logRef },
   m_Memory{ memoryRef },
   m_Platform{ platformRef }
{
    // m_lineByLine = true;
    std::cout << std::hex;

    m_Registers.set_af(0x01B0);
    m_Registers.set_bc(0x0013);
    m_Registers.set_de(0x00D8);
    m_Registers.set_hl(0x014D);

    setupTables();
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
    return m_Memory.readByte(address);
}
void CPU::writeByte(word_t address, byte_t value)
{
    //reset the current scanline if the game tries to write to it
    if (address == r_LY)
    {
        m_log(LOG_ERROR) << "Game Attemping to write to r_LY from CPU" << "\n";
        std::exit(EXIT_FAILURE);
        // m_Memory.writeByte(r_LY, 0);
        // m_ScanlineCounter = 456; //number of cycles per scanline
    }
    else if (address == r_DMAT)
    {
        initiateDMATransfer(value);
    }
    else if (address == r_TMC)
    {
        m_log(LOG_DEBUG) << "Writing new TMC value:" << +value << "\n";

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
            m_log(LOG_INFO) << "Timer reset!" << "\n";
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
    m_log(LOG_INFO) << "Starting interupt " << interupt << "!" << "\n";
    m_InteruptsEnabled = false;

    byte_t requests = readByte(c_INTERUPTS_REQ_ADDRESS);
    resetBit(requests, interupt);
    writeByte(c_INTERUPTS_REQ_ADDRESS, requests);

    push(m_PC);
    switch(interupt)
    {
        case 0: m_PC = c_VBLANK_INTERUPT; break;
        case 1: m_PC = c_LCD_INTERUPT; break;
        case 2: m_PC = c_TIMER_INTERUPT; break;
        case 3: m_PC = c_JOYPAD_INTERUPT; break;
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