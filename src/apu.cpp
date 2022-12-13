#include "inc/apu.h"
#include "inc/bitfuncs.h"
#include "inc/channels.h"

APU::APU(MemoryBus& memoryRef)
    : m_Memory {memoryRef}
{
    m_Channels.emplace_back(new SweepChannel(m_Memory,r_NR10,r_NR11,r_NR12,r_NR13,r_NR14));
    m_Channels.emplace_back(new PulseChannel(m_Memory,r_NR21,r_NR22,r_NR23,r_NR24));
    m_Channels.emplace_back(new WaveChannel(m_Memory,r_NR30,r_NR31,r_NR32,r_NR33,r_NR34));
    m_Channels.emplace_back(new NoiseChannel(m_Memory,r_NR41,r_NR42,r_NR43,r_NR44));
}


void APU::update(int clocks)
{
    for (auto& channel : m_Channels)
        channel->update(clocks);
}

/**
 * @brief (If enabled) Maps a digital value [0-0xF] to the range [-1,1], 
 * via linear interpolation, with 0 being mapped to 1. 
 * 
 * @param waveform 
 * @return int 
 */
double APU::dac(int waveform)
{
    static const double maxIn = 15.0;
    static const double minOut = -1.0;
    static const double maxOut = 1.0;

    auto lambda = waveform / maxIn;
    auto out = maxOut + lambda * (minOut-maxOut);
    return out;
}

/**
 * @brief 
 * 
 * @param channel 
 * @return double 
 */
double APU::fade(int channel)
{
    auto& val {m_LatestChannelOut[channel]};
    if (val==0)
        return 0;

    if (val < 0)
    {
        double diff {std::max(.025, val*-.1)};
        val = std::min(0., val+diff);
    }
    else
    {
        double diff {std::max(.025, val*.1)};
        val = std::max(0., val-diff);
    }
    
    return val;
}


/**
 * @brief 
 * 
 * @return std::pair<double,double> <Left, Right>
 */
std::pair<double,double> APU::mixer()
{
    std::pair<double,double> analogOut{};

    byte_t nr51 {m_Memory.readByte(r_NR51)};

    for (int i {0}; i<4; ++i)
    {
        double value{};
        if (m_Channels[i]->dacEnabled())
        {
            value = dac(m_Channels[i]->generator());
            m_LatestChannelOut[i] = value;
        }
        else
            value = fade(i);
        
        //right output
        if (testBit(nr51, i))
            analogOut.second += value;
        //left output
        if (testBit(nr51, 4+i))
            analogOut.first += value;
    }

    //then scale by master volume
    byte_t nr50 {m_Memory.readByte(r_NR50)};
    int rightVolume {extractBits(nr50,0,3) + 1};
    int leftVolume {extractBits(nr50,4,3) + 1};

    analogOut.first *= leftVolume;
    analogOut.second *= rightVolume;

    //then each channel goes through HPF
    


    return analogOut;
}

/**
 * @brief events occour every n ticks, 
 * 8 - envelope sweep, 2 - sound length, 4 - ch1 freq sweep
 */
void APU::incrementDivAPU()
{
    ++m_divAPUCounter;

    // sound_length timers increase here
    if (m_divAPUCounter%2==0)
    {
        for (auto& channel : m_Channels)
            channel->incrementLenTimer();
    }
    // ch1 freq sweep 
    if (m_divAPUCounter%4==0)
    {
        auto* channel = dynamic_cast<SweepChannel*>(m_Channels[0].get());
        if (!channel)
            throw std::runtime_error("Null PTR at m_channels[0]");

        channel->sweepIteration();
    }
    // envelope sweep channels 0,1,3
    if (m_divAPUCounter%8==0)
    {
        for (auto& channel : m_Channels)
        {
            auto* envelope = dynamic_cast<Envelope*>(channel.get());
            if (envelope)
                envelope->envelopeIteration();
        }
    }
}

bool APU::soundEnabled()
{
    byte_t NR52 {m_Memory.readByte(r_NR52)};
    return testBit(NR52, 7);
}