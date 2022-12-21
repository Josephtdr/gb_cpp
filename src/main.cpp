#include "inc/emulator.h"

#include <functional> // std::function
#include <unordered_map> // std::unordered_map


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

    Emulator emu {romFilename, settings};
    
    emu.m_log(LOG_INFO) << std::hex << "Starting up!" << "\n";

    Vsync vsync{};
    while(!emu.m_Platform.getExit())
    {
        emu.performFrame();
    }
}