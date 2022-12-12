#include "inc/channel.h"
#include "inc/bitfuncs.h"


Channel::Channel(MemoryBus& memoryRef)
    : m_Memory{memoryRef}
{}

bool Channel::dacEnabled() const
{
    return m_DacEnabled;
}
bool Channel::channelEnabled() const
{
    return m_ChannelEnabled;
}


const byte_t r_NR10 = 0xFF10; //Channel 1 sweep
const byte_t r_NR11 = 0xFF11; //Channel 1 length timer & duty cycle
const byte_t r_NR12 = 0xFF12; //Channel 1 volume & envelope
const byte_t r_NR13 = 0xFF13; //Channel 1 wavelength low 
const byte_t r_NR14 = 0xFF14; //Channel 1 wavelength high (0-2) & control


Channel_1::Channel_1(MemoryBus& memoryRef)
    : Channel{memoryRef}
{}

int Channel_1::generator()
{
    if (!channelEnabled())
        return 0;
    
    int digitalVal{};


    //generation!! here!!



    return digitalVal;
}


void Channel_1::update(int clocks)
{

    //increment duty step at 8x channels freq

    //use m_SweepPace

}

void Channel_1::sweepIteration()
{
    static const word_t c_MAX_CH1_WAVELEN = 0x7FF;

    byte_t nr10 {m_Memory.readByte(nr10)};
    bool addition {testBit(nr10, 3)};
    int slopeControl {extractBits(nr10,0,3)};
    
    word_t wavelen {getWaveLen()};
    int divisor {static_cast<int>(pow(2,slopeControl))};
    
    if (addition)
    {
        wavelen += (wavelen/divisor);
        if (wavelen > c_MAX_CH1_WAVELEN)
            m_ChannelEnabled = false;
    }
    else
        wavelen -= (wavelen/divisor);    
    
    if (slopeControl!=0)
        setWaveLen(wavelen);
}

void Channel_1::trigger()
{
    m_DutyStep = 0;
    byte_t nr11 {m_Memory.readByte(r_NR11)};
    byte_t nr12 {m_Memory.readByte(r_NR12)};
    byte_t nr14 {m_Memory.readByte(r_NR14)};

    m_InitialLengthTimer = extractBits(nr11, 0, 6);

    m_EnvelopeSweepPace = extractBits(nr12, 0, 3);
    m_EnvelopeIncrease = testBit(nr12, 3);
    m_InitialEnvelopeVolume = extractBits(nr12, 4, 4);

    bool enable = (m_InitialEnvelopeVolume==0 && !m_EnvelopeIncrease);
    m_DacEnabled = enable;
    m_ChannelEnabled = enable;
}

void Channel_1::incrementLenTimer()
{
    bool soundLenEnabled = testBit(m_Memory.readByte(r_NR14), 6);
    if (!soundLenEnabled)
        return;

    ++m_InitialLengthTimer;

    if (m_InitialLengthTimer >= 64)
        m_ChannelEnabled = false;
}


byte_t Channel_1::getVolume() const
{

}

word_t Channel_1::getWaveLen() const
{
    byte_t lsb {m_Memory.readByte(r_NR13)};
    byte_t msb = m_Memory.readByte(r_NR14) & 0b111;
    return msb | lsb;
}
void Channel_1::setWaveLen(word_t waveLen)
{
    byte_t lsb = waveLen & 0xFF;
    byte_t msb = waveLen >> 8;
    byte_t nr14 = m_Memory.readByte(r_NR14) & 0b11111000;

    m_Memory.writeByte(r_NR13, lsb);
    m_Memory.writeByte(r_NR14, nr14 | msb);
}
