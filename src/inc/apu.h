#pragma once

#include "consts.h"
#include "memoryBus.h"
#include "channel.h"

const byte_t r_NR52 = 0xFF26; //sound on off (bit 7 global, 0-3=ch 1-4)
const byte_t r_NR51 = 0xFF25; //sound panning
const byte_t r_NR50 = 0xFF24; //master volume 1-8, vin panning




class APU
{
private:
    MemoryBus& m_Memory;

    byte_t m_divAPUCounter{};
    bool m_divTrigger{};
    std::vector<std::__shared_ptr<Channel>> m_Channels{};
    std::vector<double> m_LatestChannelOut{}; 

public:
    APU(MemoryBus& memoryRef);

    void update(int clocks);
    void incrementDivAPU();

private:
    bool soundEnabled();

    double dac(int digitalValue);
    double fade(int channel);

    // struct analogOut{ double left{}; double right{};};

    std::pair<double,double> mixer();

};