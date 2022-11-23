#include "cpu.h"


void CPU::frameUpdate()
{
    int cyclesThisUpdate = 0;

    while(cyclesThisUpdate < c_MAX_CYCLES_PER_UPDATE) 
    {
        int cycles{ cycle() };
        cyclesThisUpdate += cycles;
        updateTimers(cycles);
        // UpdateGraphics(cycles);
        // DoInterupts();
    }
    // RenderScreen();
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
            // reset m_TimerTracer to the correct value
            setClockFreq();

            // timer about to overflow
            if (m_Memory.readByte(c_TIMA) == 255)
            {
                m_Memory.writeByte(c_TIMA,m_Memory.readByte(c_TMA)) ;
                RequestInterupt(2);
            }
            else
            {
                m_Memory.writeByte(c_TIMA, m_Memory.readByte(c_TIMA)+1) ;
            }
        }
    }
}

bool CPU::isClockEnabled() const
{
    return (m_Memory.readByte(c_TMC) & (1<<2));
}
byte_t CPU::getClockFreq() const
{
    return m_Memory.readByte(c_TMC) & 0b11u;
}
void CPU::setClockFreq()
{
    byte_t freq = getClockFreq();
    switch (freq)
    {
        case 0: m_TimerCounter = 1024 ; break ; // freq 4096
        case 1: m_TimerCounter = 16 ; break ;// freq 262144
        case 2: m_TimerCounter = 64 ; break ;// freq 65536
        case 3: m_TimerCounter = 256 ; break ;// freq 16382
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