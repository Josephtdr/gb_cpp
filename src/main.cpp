#include "inc/cpu.h"
#include "inc/ppu.h"
#include "inc/platform.h"
#include "inc/BSlogger.h"

#include <iostream>
#include <chrono> // for std::chrono functions
#include <cstdio>

//https://www.learncpp.com/cpp-tutorial/timing-your-code/
class Timer
{
private:
    // Type aliases to make accessing nested type easier
    using Clock = std::chrono::steady_clock;
    using Second = std::chrono::duration<double, std::ratio<1> >;

    std::chrono::time_point<Clock> m_beg{ Clock::now() };
    double frameRate{ 1.0 / 60.0 }; //in seconds

public:
    void reset()
    {
        m_beg = Clock::now();
    }
    double elapsed() const
    {
        return std::chrono::duration_cast<Second>(Clock::now() - m_beg).count();
    }
    bool nextFrameReady() const
    {
        return elapsed() >= frameRate;
    }
};

std::string getHexString(byte_t byte)
{
    
}
std::string getHexString(word_t word)
{



}


#include  <iomanip>
void doctorLog(CPU& cpu, MemoryBus& memory)
{
    std::cout << std::hex << std::uppercase << std::setfill('0') <<
        "A:" << std::setw(2) << +cpu.m_Registers.a << " " << 
        "F:" << std::setw(2) << +cpu.m_Registers.f << " " <<
        "B:" << std::setw(2) << +cpu.m_Registers.b << " " <<
        "C:" << std::setw(2) << +cpu.m_Registers.c << " " <<
        "D:" << std::setw(2) << +cpu.m_Registers.d << " " <<
        "E:" << std::setw(2) << +cpu.m_Registers.e << " " <<
        "H:" << std::setw(2) << +cpu.m_Registers.h << " " <<
        "L:" << std::setw(2) << +cpu.m_Registers.l << " " <<
        "SP:" << std::setw(4) << +cpu.m_SP << " " <<
        "PC:" << std::setw(4) << +cpu.m_PC << " " <<
        "PCMEM:"<< std::setw(2) << +memory.readMemForDoctor(cpu.m_PC) << "," <<
                   std::setw(2) << +memory.readMemForDoctor(cpu.m_PC+1) << "," <<
                   std::setw(2) << +memory.readMemForDoctor(cpu.m_PC+2) << "," <<
                   std::setw(2) << +memory.readMemForDoctor(cpu.m_PC+3) <<"\n";
}

void frameUpdate(CPU& cpu, PPU& ppu, logger& log, MemoryBus& memory)
{
    int cyclesThisUpdate = 0;

    while(cyclesThisUpdate < c_MAX_CYCLES_PER_UPDATE)
    {
        if (!memory.m_bootRomLoaded)
            doctorLog(cpu, memory);
        // cpu.m_lineByLine = !memory.m_bootRomLoaded;

        if (!cpu.isHalted())
        {
            int cycles{ cpu.cycle() };
            cyclesThisUpdate += cycles;
            cpu.updateTimers(cycles);
            cpu.updateJoypad();
            ppu.updateGraphics(cycles);
            // if (cpu.m_lineByLine)
            //     getchar();
        }
        cpu.interupts();
    }
    ppu.renderScreen();
}

int main(int argc, char *argv[])
{
    freopen( "logfordoc.txt", "w", stdout );

    if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <ROM>\n";
		std::exit(EXIT_FAILURE);
	}
    char const* romFilename = argv[1];

    logger log{ std::clog, __PRETTY_FUNCTION__ };
    log.set_log_level(LOG_INFO);
    log(LOG_INFO) << "Starting up!" << "\n";

    MemoryBus memory{ log };
    memory.loadGame(romFilename);
    log(LOG_INFO) << "Game Loaded!" << "\n";

    Platform platform{ memory.getTitle().c_str(),c_VIDEO_WIDTH,c_VIDEO_HEIGHT,2 };

    CPU cpu{ memory,log,platform };
    log(LOG_INFO) << "CPU initialised!" << "\n";
    PPU ppu{ memory,log,platform };
    log(LOG_INFO) << "PPU initialised!" << "\n";

    

    Timer timer{};

    while(!platform.getExit())
    {
        if(timer.nextFrameReady())
        {
            timer.reset();
            frameUpdate(cpu, ppu, log, memory);
        }
    }
}