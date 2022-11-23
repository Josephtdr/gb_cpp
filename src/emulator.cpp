#include "emulator.h"


void Emulator::frameUpdate()
{
    int cyclesThisUpdate = 0;

    while(cyclesThisUpdate < c_MAX_CYCLES_PER_UPDATE) 
    {
        cyclesThisUpdate += m_CPU.cycle();
        // UpdateTimers(cycles);
        // UpdateGraphics(cycles);
        // DoInterupts();
    }
    // RenderScreen();
}
