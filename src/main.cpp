#include "inc/cpu.h"
#include "inc/ppu.h"
#include "inc/apu.h"
#include "inc/platform.h"
#include "inc/BSlogger.h"

#include <iostream>
#include <chrono> // for std::chrono functions
#include <cstdio>
#include <functional> // std::function
#include <unordered_map> // std::unordered_map

//https://www.learncpp.com/cpp-tutorial/timing-your-code/
class Vsync
{
private:
    // Type aliases to make accessing nested type easier
    using Clock = std::chrono::steady_clock;
    using Second = std::chrono::duration<double, std::ratio<1> >;

    std::chrono::time_point<Clock> m_beg{ Clock::now() };
    double frameRate{ 1.0 / 59.7 }; //in seconds

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

void frameUpdate(CPU& cpu, PPU& ppu, APU& apu, logger& log, MemoryBus& memory, Settings& settings)
{
    int cyclesThisUpdate {};
    int cycles {};

    while(cyclesThisUpdate < c_MAX_CYCLES_PER_UPDATE)
    {
        cycles = cpu.interupts();

        if (cpu.isHalted()) [[unlikely]]
            cycles += 4;
            
        else if (cpu.isStopped()) [[unlikely]]
        {
            cpu.updateJoypad(); 
            continue;
        }
        else
            cycles += cpu.cycle();
        
        cyclesThisUpdate += cycles;
        cpu.update(cycles);
        ppu.updateGraphics(cycles);
        if (!settings.disableAudio)
            apu.update(cycles);
    }
    ppu.renderScreen();
}


typedef std::function<void(Settings&)> NoArgHandle;

const std::unordered_map<std::string, NoArgHandle> NoArgs {
  {"--traceLog", [](Settings& s) { s.traceLog = true; }},
  {"-tl", [](Settings& s) { s.traceLog = true; }},
  {"--bootRom", [](Settings& s) { s.bootRom = true; }},
  {"-br", [](Settings& s) { s.bootRom = true; }},
  {"--disableAudio", [](Settings& s) { s.disableAudio = true; }},
  {"-da", [](Settings& s) { s.disableAudio = true; }},
};

int main(int argc, char *argv[])
{
    if (argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " <ROM>\n";
		std::exit(EXIT_FAILURE);
	}
    char const* romFilename = argv[1];

    Settings settings{};
    for (int i {2}; i < argc; ++i)
    {
        std::string opt {argv[i]};
        if(auto j {NoArgs.find(opt)}; j != NoArgs.end())
            j->second(settings);
    }
    if (settings.traceLog)
        freopen( "trace.txt", "w", stdout );

    logger log{ std::cout, __PRETTY_FUNCTION__ };
    log.set_log_level(LOG_INFO);
    MemoryBus memory{ log };
    memory.loadGame(romFilename, settings.bootRom);
    Platform platform{ memory.getTitle().c_str(),c_VIDEO_WIDTH,c_VIDEO_HEIGHT,2 };
    PPU ppu{ memory,log,platform };
    APU apu{ memory, platform };
    CPU cpu{ memory,log,platform,ppu,apu,settings };
    
    log(LOG_INFO) << std::hex << "Starting up!" << "\n";

    Vsync vsync{};
    while(!platform.getExit())
    {
        if(vsync.nextFrameReady())
        {
            vsync.reset();
            frameUpdate(cpu, ppu, apu, log, memory, settings);
        }
    }
}