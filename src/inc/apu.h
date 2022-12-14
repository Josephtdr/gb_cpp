#pragma once

#include "consts.h"
#include "memoryBus.h"
#include "channels.h"
#include "platform.h"

struct Sample{
    float left; float right;
};
const int c_SAMPLE_BUFFER_SIZE = 2048;

class APU
{
private:
    MemoryBus& m_Memory;
    Platform& m_Platform;
    bool m_Enabled{};

    int m_SampleRate{}; // 4194304/44100 = 95 clocks (truncated)
    int m_SampleCounter{};
    std::vector<float> m_sampleBuffer{};
    std::vector<Sample> m_RAW_SampleBuffer{};
    int m_RAW_SamplePointer{};


    byte_t m_divAPUCounter{};
    bool m_divTrigger{};
    std::vector<std::unique_ptr<Channel>> m_Channels{};
    std::vector<float> m_LatestChannelOut{0,0,0,0}; 

public:
    APU(MemoryBus& memoryRef, Platform& platformRef);

    void update(int clocks);
    void renderAudio();
    void incrementDivAPU();
    void toggle(byte_t value);
    void control(int channel, byte_t value);

private:
    bool soundEnabled();

    float dac(int digitalValue);
    float fade(int channel);

    Sample mixer();
    void takeRawSample();
    void takeSample();
};