#include "inc/cpu.h"

#include <iostream> 
#include <stdexcept>

CPU::CPU()
 : m_PC{0},
   m_SP{ c_TOP_OF_STACK },
   m_log{ std::cout, __PRETTY_FUNCTION__ }
{
    m_log.set_log_level(LOG_DEBUG);
    // m_lineByLine = true;
    std::cout << std::hex;

    m_Registers.set_af(0x01B0);
    m_Registers.set_bc(0x0013);
    m_Registers.set_de(0x00D8);
    m_Registers.set_hl(0x014D);

    setupTables();
}

void CPU::frameUpdate()
{
    int cyclesThisUpdate = 0;

    while(cyclesThisUpdate < c_MAX_CYCLES_PER_UPDATE)
    {
        if (!m_Halted)
        {
            int cycles{ cycle() };
            cyclesThisUpdate += cycles;
            updateTimers(cycles);
            // UpdateGraphics(cycles);
            if (m_lineByLine)
            getchar();
        }
        interupts();
    }
    m_log(LOG_ERROR) << "Frame finished!" << "\n";
    // RenderScreen();
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

void CPU::loadGame(const char* fileName)
{
    m_Memory.loadGame(fileName);
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
            setClockFreq();

            // timer about to overflow
            if (m_Memory.readByte(c_TIMA_ADDRESS) == 255)
            {
                m_Memory.writeByte(c_TIMA_ADDRESS,m_Memory.readByte(c_TMA_ADDRESS)) ;
                requestInterupt(2);
            }
            else
            {
                m_Memory.writeByte(c_TIMA_ADDRESS, m_Memory.readByte(c_TIMA_ADDRESS)+1) ;
            }
        }
    }
}

bool CPU::isClockEnabled() const
{
    return testBit(m_Memory.readByte(c_TMC_ADDRESS), 2);
}
byte_t CPU::getClockFreq() const
{
    return m_Memory.readByte(c_TMC_ADDRESS) & 0b11u;
}
void CPU::setClockFreq()
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
        m_Memory.incrementDivRegister();
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
        byte_t requests = m_Memory.readByte(c_INTERUPTS_REQ_ADDRESS);
        byte_t enabled = m_Memory.readByte(c_INTERUPTS_ENABLED_ADDRESS);
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
        byte_t requests = m_Memory.readByte(c_INTERUPTS_REQ_ADDRESS);
        byte_t enabled = m_Memory.readByte(c_INTERUPTS_ENABLED_ADDRESS);

        if (requests & enabled)
            m_Halted = false;
    }
}

void CPU::requestInterupt(int interupt) //0,1,2,4
{
    byte_t requests = m_Memory.readByte(c_INTERUPTS_REQ_ADDRESS);
    //set relevent bit
    setBit(requests, interupt);
    m_Memory.writeByte(c_INTERUPTS_REQ_ADDRESS, requests);
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

    byte_t requests = m_Memory.readByte(c_INTERUPTS_REQ_ADDRESS);
    resetBit(requests, interupt);
    m_Memory.writeByte(c_INTERUPTS_REQ_ADDRESS, requests);

    push(m_PC);
    switch(interupt)
    {
        case 0: m_PC = c_VBLANK_INTERUPT; break;
        case 1: m_PC = c_LCD_INTERUPT; break;
        case 2: m_PC = c_TIMER_INTERUPT; break;
        case 3: m_PC = c_JOYPAD_INTERUPT; break;
    }
}