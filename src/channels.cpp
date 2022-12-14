#include "inc/channels.h"
#include "inc/bitfuncs.h"

Channel::Channel(MemoryBus& memoryRef, 
    word_t r1, word_t r2, word_t r3, word_t r4,
    int idx,int baseLenTimer, int lenBitSize)
    : m_Memory{memoryRef}, m_NRx1{r1}, m_NRx2{r2}, m_NRx3{r3}, m_NRx4{r4}, 
      m_ChannelIdx{idx}, m_baseLenTimer{baseLenTimer}, m_lenBitSize{lenBitSize}
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
    bool soundLenEnabled = testBit(m_Memory.readByte(m_NRx4), 6);
}
void Channel::control(byte_t value)
{
    m_SoundLenEnabled = testBit(value, 6);
    if (testBit(value, 7))
        trigger();
}
void Channel::incrementLenTimer()
{
    if (!m_SoundLenEnabled)
        return;

    --m_InitialLengthTimer;

    if (m_InitialLengthTimer <= 0)
        toggle(false);
}
void Channel::toggle(bool on)
{
    if (m_ChannelEnabled!=on)
    {
        m_ChannelEnabled = on;

        auto nr52 {m_Memory.readByte(r_NR52)};

        if (on)
            setBit(nr52, m_ChannelIdx);
        else
            resetBit(nr52, m_ChannelIdx);
    }
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
    toggle(enable);
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
    : Channel{memoryRef,r1,r2,r3,r4,1}, Timer{4}, Envelope{}
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
    : Channel{memoryRef,r1,r2,r3,r4,0}, PulseChannel{memoryRef,r1,r2,r3,r4}, m_NRx0{r0}
{
}

void SweepChannel::trigger()
{
    PulseChannel::trigger();

    byte_t nrx0 {m_Memory.readByte(m_NRx0)};
    m_SweepPace = extractBits(nrx0, 4, 3);
    m_SweepCounter = m_SweepPace;
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
    //dont iterate if pace is 0, but reload to check its not changed
    if (m_SweepPace == 0)
    {
        byte_t nrx0 {m_Memory.readByte(m_NRx0)};
        m_SweepPace = extractBits(nrx0, 4, 3);
        m_SweepCounter = m_SweepPace;
        return;
    }

    //wait for sweep counter to hit 0 to perform an iteration
    --m_SweepCounter;
    if (m_SweepCounter > 0)
        return;
    m_SweepCounter = m_SweepPace;

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
            toggle(false);
    }
    else
        wavelen -= (wavelen/divisor);    
    
    if (slopeControl!=0)
        setFrequency(wavelen);
}

//******************************************************************************
WaveChannel::WaveChannel(MemoryBus& memoryRef, 
        word_t r0, word_t r1, word_t r2, word_t r3, word_t r4)
    : Channel{memoryRef,r1,r2,r3,r4,2,256,8}, Timer{2}, m_NRx0{r0}
{}

int WaveChannel::generator()
{   
    if (!channelEnabled())
        return 0;
    
    
    // muted 
    if (m_volumeLevel==0)
        return 0;

    int digitalVal {m_sampleBuffer};
    digitalVal = digitalVal >> (m_volumeLevel-1);

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
    Channel::trigger();

    setPeriod();

    //TODO possibly change on write to address
    m_volumeLevel = extractBits(m_Memory.readByte(m_NRx2),5,2);

    m_sampleIndex = 0;
    m_DacEnabled = true;
    toggle(true);
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
    : Channel{memoryRef,r1,r2,r3,r4,3}, Envelope{}
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
