#pragma once

#include "consts.h"
#include "memoryBus.h"
#include "channels.h"
#include "platform.h"

struct Sample{
    int left; int right;
};

class APU
{
private:
    MemoryBus& m_Memory;
    Platform& m_Platform;
    bool m_Enabled{};

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
    void toggle(byte_t value);
    void control(int channel, byte_t value);

private:
    bool soundEnabled();

    double dac(int digitalValue);
    double fade(int channel);

    Sample mixer();
    Sample sample();
    void flushBuffer();
};