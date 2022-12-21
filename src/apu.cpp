#include "inc/apu.h"
#include "inc/bitfuncs.h"
#include "inc/channels.h"

constexpr int c_SAMPLES_PER_FRAME {(44100/60)*2}; //* 2, for each channel

APU::APU(MemoryBus& memoryRef, Platform& platformRef)
  : m_Memory {memoryRef}, 
    m_Platform {platformRef}, 
    m_SampleRate {4194304/44100},
    m_SampleCounter {-(m_SampleRate * 4)}, // to enable sampling 1 sample later?
    m_Enabled {true},
    m_RAW_SampleBuffer(200)
{
    m_Channels.emplace_back(
        std::make_unique<SweepChannel>(m_Memory,r_NR10,r_NR11,r_NR12,r_NR13,r_NR14));
    m_Channels.emplace_back(
        std::make_unique<PulseChannel>(m_Memory,r_NR21,r_NR22,r_NR23,r_NR24));
    m_Channels.emplace_back(
        std::make_unique<WaveChannel>(m_Memory,r_NR30,r_NR31,r_NR32,r_NR33,r_NR34));
    m_Channels.emplace_back(
        std::make_unique<NoiseChannel>(m_Memory,r_NR41,r_NR42,r_NR43,r_NR44));

    m_sampleBuffer.reserve(c_SAMPLES_PER_FRAME);
}

void APU::update(int clocks)
{
    for (auto& channel : m_Channels)
        channel->update(clocks);

    takeRawSample();

    m_SampleCounter += clocks;
    while(m_SampleCounter >= m_SampleRate)
    {
        m_SampleCounter -= m_SampleRate;
        takeSample();
    }
}
void APU::takeRawSample()
{
    auto sample = mixer();
    //convert to raw PCM floating point samples
    sample.left /= 100.f;
    sample.right /= 100.f;
    
    m_RAW_SampleBuffer[m_RAW_SamplePointer++] = sample;
    if (m_RAW_SamplePointer >= 200)
        m_RAW_SamplePointer = 0;
}

void APU::takeSample()
{
    static std::vector<float> firFilter{
        -0.000136, -0.000232, 0.000580, 0.000000, -0.001169, 0.000972, 0.001276, -0.002656, -0.000000, 0.004190, -0.003192, -0.003895, 0.007626, 0.000000, -0.010944, 0.008043, 0.009535, -0.018262, -0.000000, 0.025648, -0.018898, -0.022720, 0.044799, 0.000000, -0.071809, 0.060553, 0.092332, -0.301742, 0.400000, -0.301742, 0.092332, 0.060553, -0.071809, 0.000000, 0.044799, -0.022720, -0.018898, 0.025648, -0.000000, -0.018262, 0.009535, 0.008043, -0.010944, 0.000000, 0.007626, -0.003895, -0.003192, 0.004190, -0.000000, -0.002656, 0.001276, 0.000972, -0.001169, 0.000000, 0.000580, -0.000232, -0.000136 
    };

    int start = (m_RAW_SamplePointer - m_SampleRate + 22);
    if (start < 0) start += 200;

    Sample sampleOut{};

    for (int i {}; i < 57; ++i)
    {
        int index = (start+i) % 200;

        sampleOut.left += m_RAW_SampleBuffer[index].left * firFilter[i];
        sampleOut.right += m_RAW_SampleBuffer[index].right * firFilter[i];
    }

    m_sampleBuffer.push_back(sampleOut.left);
    m_sampleBuffer.push_back(sampleOut.right);
}


void APU::renderAudio()
{
    m_Platform.updateAudio(m_sampleBuffer.data(), m_sampleBuffer.size());
    m_sampleBuffer.clear();
    m_RAW_SampleBuffer.clear();
}

/**
 * @brief 
 * 
 * @return std::pair<double,double> <Left, Right>
 */
Sample APU::mixer()
{
    Sample analogOut{};

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
        {
            value = fade(i);
        }
        //right output
        if (testBit(nr51, i))
            analogOut.right += value;
        //left output
        if (testBit(nr51, 4+i))
            analogOut.left += value;
    }

    //scale by master volume
    byte_t nr50 {m_Memory.readByte(r_NR50)};
    int rightVolume {extractBits(nr50,0,3) + 1};
    int leftVolume {extractBits(nr50,4,3) + 1};

    analogOut.left *= leftVolume;
    analogOut.right *= rightVolume;

    //then each channel goes through HPF

    return analogOut;
}

/**
 * @brief (If enabled) Maps a digital value [0-0xF] to the range [-1,1], 
 * via linear interpolation, with 0 being mapped to 1. 
 * 
 * @param waveform 
 * @return int 
 */
float APU::dac(int waveform)
{
    static const float maxIn = 15.0f;
    static const float minOut = -1.0f;
    static const float maxOut = 1.0f;

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
float APU::fade(int channel)
{
    auto& val {m_LatestChannelOut[channel]};
    if (val==0)
        return 0;

    if (val < 0)
    {
        float diff {std::max(.025f, val*-.1f)};
        val = std::min(0.f, val+diff);
    }
    else
    {
        float diff {std::max(.025f, val*.1f)};
        val = std::max(0.f, val-diff);
    }
    
    return val;
}

void APU::toggle(byte_t value)
{
    m_Enabled = testBit(value, 7);
    if (m_Enabled)
        m_Platform.playAudio();
    else
        m_Platform.pauseAudio();
}
/**
 * @brief updates a given channels Sound Length enable
 * and retriggers channel if given bits are enabeld
 * @param channelIDX channel 1 is idx 0 etc...
 * @param value 
 */
void APU::control(int channelIDX, byte_t value)
{
    auto& channel = m_Channels[channelIDX];
    channel->control(value);
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