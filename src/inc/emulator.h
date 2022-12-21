#pragma once

#include "cpu.h"
#include "ppu.h"
#include "apu.h"
#include "platform.h"
#include "BSlogger.h"

#include <iostream>
#include <chrono> // for std::chrono functions
#include <cstdio>
#include <functional> // std::function
#include <unordered_map> // std::unordered_map

class Vsync
{
private:
    // Type aliases to make accessing nested type easier
    using Clock = std::chrono::steady_clock;
    using Second = std::chrono::duration<double, std::ratio<1> >;

    std::chrono::time_point<Clock> m_beg{ Clock::now() };
    double frameRate{ 1.0 / 59.7 }; //in seconds

public:
    void reset();
    double elapsed() const;
    bool nextFrameReady() const;
};

class Emulator
{
public:
    Settings   m_Settings;
    logger     m_log;
    MemoryBus  m_Memory;
    Platform   m_Platform;
    APU        m_APU;
    CPU        m_CPU;
    PPU        m_PPU;
    Vsync      m_Vsync{};

public:
    Emulator(const char* filename, Settings& settings);
    void performFrame();

private:
    void cpuFrame();
};