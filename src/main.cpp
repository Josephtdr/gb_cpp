#include <iostream>
#include <chrono> // for std::chrono functions

#include "inc/cpu.h"
#include "inc/ppu.h"
#include "inc/platform.h"
#include "inc/BSlogger.h"

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

void frameUpdate(CPU& cpu, PPU& ppu, logger& log)
{
    int cyclesThisUpdate = 0;

    while(cyclesThisUpdate < c_MAX_CYCLES_PER_UPDATE)
    {
        if (!cpu.isHalted())
        {
            int cycles{ cpu.cycle() };
            cyclesThisUpdate += cycles;
            cpu.updateTimers(cycles);
            cpu.updateJoypads();
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
    if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <ROM>\n";
		std::exit(EXIT_FAILURE);
	}
    char const* romFilename = argv[1];

    logger log{ std::cout, __PRETTY_FUNCTION__ };
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

    

    bool quit{};
    Timer timer{};
    byte_t keypad{};

    while(!quit)
    {
        quit = platform.ProcessInput(keypad);

        if(timer.nextFrameReady())
        {
            timer.reset();
            frameUpdate(cpu, ppu, log);
        }
    }
}