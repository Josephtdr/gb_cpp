#include <iostream>

#include "cpu.h"


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

    while(!quit)
    {
        cpu.frameUpdate();




    }


}