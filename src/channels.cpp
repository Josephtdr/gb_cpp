#include "inc/channels.h"
#include "inc/bitfuncs.h"

Channel::Channel(MemoryBus& memoryRef, 
    word_t r1, word_t r2, word_t r3, word_t r4,int baseLenTimer, int lenBitSize)
    : m_Memory{memoryRef}, m_NRx1{r1}, m_NRx2{r2}, m_NRx3{r3}, m_NRx4{r4}, 
      m_baseLenTimer{baseLenTimer}, m_lenBitSize{lenBitSize}
{}

bool Channel::dacEnabled() const
{
    return m_DacEnabled;
}
bool Channel::channelEnabled() const
{
    return m_ChannelEnabled;
}
void Channel::trigger()
{
    byte_t nrx1 {m_Memory.readByte(m_NRx1)};
    m_InitialLengthTimer = m_baseLenTimer - extractBits(nrx1, 0, m_lenBitSize);
}
void Channel::incrementLenTimer()
{
    bool soundLenEnabled = testBit(m_Memory.readByte(m_NRx4), 6);
    if (!soundLenEnabled)
        return;

    --m_InitialLengthTimer;

    if (m_InitialLengthTimer <= 0)
        m_ChannelEnabled = false;
}

//******************************************************************************
Envelope::Envelope()
{}

void Envelope::envelopeIteration()
{
    if (m_EnvelopeSweepPace==0)
        return;

    ++m_EnvelopeSweepCounter;
    if (m_EnvelopeSweepCounter < m_EnvelopeSweepPace)
        return;
    m_EnvelopeSweepCounter = 0;
    
    if (m_EnvelopeIncrease)
        m_EnvelopeVolume = std::min(0xF, m_EnvelopeVolume+1);
    else
        m_EnvelopeVolume = std::max(0, m_EnvelopeVolume-1);
}

void Envelope::trigger()
{
    Channel::trigger();

    byte_t nrx2 {m_Memory.readByte(m_NRx2)};

    m_EnvelopeSweepPace = extractBits(nrx2, 0, 3);
    m_EnvelopeIncrease = testBit(nrx2, 3);
    m_EnvelopeVolume = extractBits(nrx2, 4, 4);

    bool enable = (m_EnvelopeVolume==0 && !m_EnvelopeIncrease);
    m_DacEnabled = enable;
    m_ChannelEnabled = enable;
}

//******************************************************************************
Timer::Timer(int scale)
    : m_scale {scale}
{}
word_t Timer::getFrequency()
{
    byte_t lsb {m_Memory.readByte(m_NRx3)};
    byte_t msb = m_Memory.readByte(m_NRx4) & 0x7;
    return (msb << 8) | lsb;
}
void Timer::setPeriod()
{
    int wavelength = getFrequency();

    m_period = (2048 - wavelength) * m_scale;
    m_periodClock = m_period;
}
bool Timer::updateTimer(int clocks)
{
    m_periodClock -= clocks;

    if (m_periodClock>0)
        return false;

    m_periodClock += m_period;
    return true;    
}


//******************************************************************************
PulseChannel::PulseChannel(MemoryBus& memoryRef, word_t r1, word_t r2, word_t r3, word_t r4)
    : Channel{memoryRef,r1,r2,r3,r4}, Timer{4}, Envelope{}
{}

void PulseChannel::trigger()
{
    Envelope::trigger();
    
    byte_t nrx1 {m_Memory.readByte(m_NRx1)};
    m_DutyCycle = extractBits(nrx1,6,2);
    m_DutyPointer = 0;

    setPeriod();
}

int PulseChannel::generator()
{
    static const std::vector<byte_t> waveforms {
        0b00000001, 0b10000001, 0b10000111, 0b01111110
    };

    bool raised {testBit(waveforms[m_DutyCycle], m_DutyPointer)}; 
    if (!raised)
        return 0;
    else
        return m_EnvelopeVolume;
}

void PulseChannel::update(int clocks)
{
    if (!updateTimer(clocks))
        return;

    ++m_DutyPointer;
    if (m_DutyPointer >= 8)
        m_DutyPointer = 0;
}


//******************************************************************************
SweepChannel::SweepChannel(MemoryBus& memoryRef, word_t r0, word_t r1, word_t r2, word_t r3, word_t r4)
    : Channel{memoryRef,r1,r2,r3,r4}, PulseChannel{memoryRef,r1,r2,r3,r4}, m_NRx0{r0}
{
}

void SweepChannel::trigger()
{
    PulseChannel::trigger();

    byte_t nrx0 {m_Memory.readByte(m_NRx0)};
    m_SweepPace = extractBits(nrx0, 4, 3);
}

void SweepChannel::setFrequency(word_t waveLen)
{
    byte_t lsb = waveLen & 0xFF;
    byte_t msb = (waveLen >> 8) & 0x7;
    byte_t nrx4 = m_Memory.readByte(m_NRx4) & 0xF8;

    m_Memory.writeByte(m_NRx3, lsb);
    m_Memory.writeByte(m_NRx4, nrx4 | msb);

    setPeriod();
}

void SweepChannel::sweepIteration()
{
    static const word_t c_MAX_CH1_WAVELEN = 0x7FF;

    byte_t nr10 {m_Memory.readByte(m_NRx0)};
    bool addition {testBit(nr10, 3)};
    int slopeControl {extractBits(nr10,0,3)};
    
    word_t wavelen {getFrequency()};
    int divisor {static_cast<int>(pow(2,slopeControl))};
    
    if (addition)
    {
        wavelen += (wavelen/divisor);

        //overflow turns off channel
        if (wavelen > c_MAX_CH1_WAVELEN)
            m_ChannelEnabled = false;
    }
    else
        wavelen -= (wavelen/divisor);    
    
    if (slopeControl!=0)
        setFrequency(wavelen);
}

//******************************************************************************
WaveChannel::WaveChannel(MemoryBus& memoryRef, 
        word_t r0, word_t r1, word_t r2, word_t r3, word_t r4)
    : Channel{memoryRef,r1,r2,r3,r4,256,8}, Timer{2}, m_NRx0{r0}
{}

int WaveChannel::generator()
{   
    if (!channelEnabled())
        return 0;
    
    int volumeType = extractBits(m_Memory.readByte(m_NRx2),5,2);
    // muted 
    if (volumeType==0)
        return 0;

    int digitalVal {m_sampleBuffer};
    digitalVal = digitalVal >> (volumeType-1);

    return digitalVal;
}

void WaveChannel::update(int clocks)
{
    if (!updateTimer(clocks))
        return;
    else
        readNextWaveForm();
}
void WaveChannel::trigger()
{
    setPeriod();

    m_sampleIndex = 0;
    m_DacEnabled = true;
    m_ChannelEnabled = true;
}

void WaveChannel::readNextWaveForm()
{
    static const word_t waveFormStart = 0xFF30;

    ++m_sampleIndex;
    if (m_sampleIndex >= 32)
        m_sampleIndex = 0;

    byte_t wave = m_Memory.readByte(waveFormStart + (m_sampleIndex/2));

    if (m_sampleIndex%2==0)
        wave = wave >> 4;
    else
        wave = wave & 0xF;

    m_sampleBuffer = wave;
}

//******************************************************************************
NoiseChannel::NoiseChannel(MemoryBus& memoryRef, word_t r1, word_t r2, word_t r3, word_t r4)
    : Channel{memoryRef,r1,r2,r3,r4}, Envelope{}
{
}

int NoiseChannel::generator()
{
    return 0;
}
void NoiseChannel::update(int clocks)
{
    
}
void NoiseChannel::trigger()
{
    Envelope::trigger();

    
}