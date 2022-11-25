#include <iostream>
#include <chrono> // for std::chrono functions

#include "unistd.h"
#include "cpu.h"


class Timer
{
private:
    // Type aliases to make accessing nested type easier
    using Clock = std::chrono::steady_clock;
    using Second = std::chrono::duration<double, std::ratio<1> >;

    std::chrono::time_point<Clock> m_beg{ Clock::now() };

    double frameRate{ 0.016666666666666 }; //in seconds

public:

    void reset()
    {
        m_beg = Clock::now();
    }

    double elapsed() const
    {
        return std::chrono::duration_cast<Second>(Clock::now() - m_beg).count();
    }

    double nextFrameIn() const
    {
        return frameRate - elapsed();
    }
};

int main(int argc, char *argv[])
{
    if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <ROM>\n";
		std::exit(EXIT_FAILURE);
	}

    std::cout << "Starting up!" << "\n";

    char const* romFilename = argv[1];

    CPU cpu{};

    std::cout << "CPU initialised!" << "\n";

    cpu.loadGame(romFilename);

    bool quit{};
    Timer t{};

    while(!quit)
    {
        t.reset();
        cpu.frameUpdate();
        sleep(t.nextFrameIn()); //sleep until next frame cycle can start
    }


}