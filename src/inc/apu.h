#pragma once

#include "consts.h"
#include "memoryBus.h"
#include "channels.h"
#include "platform.h"

const word_t r_NR52 = 0xFF26; //sound on off (bit 7 global, 0-3=ch 1-4)
const word_t r_NR51 = 0xFF25; //sound panning
const word_t r_NR50 = 0xFF24; //master volume 1-8, vin panning


struct Sample{
    int left; int right;
};

class APU
{
private:
    MemoryBus& m_Memory;
    Platform& m_Platform;

    int m_SampleRate{}; //4194304/48000 = 87 (rounded)
    int m_SampleCounter{};
    std::vector<Sample> m_sampleBuffer{1024};


    byte_t m_divAPUCounter{};
    bool m_divTrigger{};
    std::vector<std::__shared_ptr<Channel>> m_Channels{};
    std::vector<double> m_LatestChannelOut{}; 

public:
    APU(MemoryBus& memoryRef, Platform& platformRef);

    void update(int clocks);
    void incrementDivAPU();

private:
    bool soundEnabled();

    double dac(int digitalValue);
    double fade(int channel);

    Sample mixer();
    Sample sample();
    void flushBuffer();
};