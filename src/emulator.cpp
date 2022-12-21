#include "inc/emulator.h"

//https://www.learncpp.com/cpp-tutorial/timing-your-code/
void Vsync::reset()
{
    m_beg = Clock::now();
}
double Vsync::elapsed() const
{
    return std::chrono::duration_cast<Second>(Clock::now() - m_beg).count();
}
bool Vsync::nextFrameReady() const
{
    return elapsed() >= frameRate;
}


Emulator::Emulator(const char* filename, Settings& settings)
    : 
    m_Settings{ settings },  
    m_log{ std::cout, __PRETTY_FUNCTION__ },
    m_Memory{ m_log },
    m_Platform{ m_Memory.getTitle().c_str(),c_VIDEO_WIDTH,c_VIDEO_HEIGHT,2 },
    m_PPU{ m_Memory,m_log,m_Platform },
    m_APU{ m_Memory, m_Platform },
    m_CPU{ m_Memory,m_log,m_Platform,m_PPU,m_APU,settings }
{
    m_log.set_log_level(LOG_INFO);
    m_Memory.loadGame(filename, settings.bootRom);
}

void Emulator::performFrame()
{
    //Perform one frame of emulation then wait for vsync
    m_Vsync.reset();
    cpuFrame();
    while(!m_Vsync.nextFrameReady()) {}

    //Then output next screen and audio
    m_PPU.renderScreen();
    m_APU.renderAudio();

}

void Emulator::cpuFrame()
{
    int cyclesThisUpdate {};
    int cycles {};

    while(cyclesThisUpdate < c_MAX_CYCLES_PER_UPDATE)
    {
        cycles += m_CPU.interupts();

        if (m_CPU.isHalted()) [[unlikely]]
            cycles += 4;
            
        else if (m_CPU.isStopped()) [[unlikely]]
        {
            m_CPU.updateJoypad(); 
            continue;
        }
        else
            cycles += m_CPU.cycle();
        
        cyclesThisUpdate += cycles;

        while(cycles > 0)
        {
            cycles -= 4;
            m_CPU.update(4);
            m_PPU.updateGraphics(4);
            if (!m_Settings.disableAudio)
                m_APU.update(4);
        } 
    }
}