#include "inc/cpu.h"
#include "inc/ppu.h"
#include "inc/platform.h"
#include "inc/BSlogger.h"

#include <iostream>
#include <chrono> // for std::chrono functions
#include <cstdio>
#include <functional> // std::function
#include <unordered_map> // std::unordered_map

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

void frameUpdate(CPU& cpu, PPU& ppu, logger& log, MemoryBus& memory, bool doclog = true)
{
    int cyclesThisUpdate = 0;

    while(cyclesThisUpdate < c_MAX_CYCLES_PER_UPDATE)
    {
        if (doclog)
            doctorLog(cpu, memory);

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


struct Settings {
  bool doctorLog {false};
  bool traceLog {false};
  bool bootRom {false};
};

typedef std::function<void(Settings&)> NoArgHandle;

const std::unordered_map<std::string, NoArgHandle> NoArgs {
  {"--doctorLog", [](Settings& s) { s.doctorLog = true; }},
  {"-dl", [](Settings& s) { s.doctorLog = true; }},
  {"--traceLog", [](Settings& s) { s.traceLog = true; }},
  {"-tl", [](Settings& s) { s.traceLog = true; }},
  {"--bootRom", [](Settings& s) { s.bootRom = true; }},
  {"-br", [](Settings& s) { s.bootRom = true; }},
};

int main(int argc, char *argv[])
{
    freopen( "logfordoc.txt", "w", stdout );
    freopen( "trace.txt", "w", stderr );

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

    logger log{ std::cerr, __PRETTY_FUNCTION__ };
    log.set_log_level(LOG_INFO);
    MemoryBus memory{ log };
    memory.loadGame(romFilename, settings.bootRom);
    Platform platform{ memory.getTitle().c_str(),c_VIDEO_WIDTH,c_VIDEO_HEIGHT,2 };
    PPU ppu{ memory,log,platform };
    CPU cpu{ memory,log,platform,ppu,settings.traceLog };
    
    log(LOG_INFO) << std::hex << "Starting up!" << "\n";

    Timer timer{};
    while(!platform.getExit())
    {
        if(timer.nextFrameReady())
        {
            timer.reset();
            frameUpdate(cpu, ppu, log, memory, settings.doctorLog);
        }
    }
}