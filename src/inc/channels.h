#pragma once

#include "consts.h"
#include "memoryBus.h"

class Channel
{
protected:
    MemoryBus& m_Memory;
    int m_ChannelIdx{};
    word_t m_NRx1{}; 
    word_t m_NRx2{}; 
    word_t m_NRx3{}; 
    word_t m_NRx4{}; 
    int m_InitialLengthTimer{};
    int m_baseLenTimer{};
    int m_lenBitSize{};
    bool m_ChannelEnabled{};
    bool m_DacEnabled{};
    bool m_SoundLenEnabled{};

    Channel(MemoryBus& memoryRef, 
        word_t r1, word_t r2, word_t r3, word_t r4, 
        int idx, int baseLenTimer=64, int lenBitSize=6);

    void toggle(bool on);
public: 
    bool channelEnabled() const;
    bool dacEnabled() const;
    void incrementLenTimer();
    void control(byte_t value);

    virtual int generator() = 0;
    virtual void update(int clocks) = 0;
    virtual void trigger();

    virtual ~Channel() = default;
};

class Envelope : virtual public Channel
{
protected:
    int m_EnvelopeSweepCounter{};
    // update only on reset
    int m_EnvelopeSweepPace{}; 
    bool m_EnvelopeIncrease{}; // false decrease
    int m_EnvelopeVolume{};

    Envelope();
public:
    void envelopeIteration();
    void trigger() override;
};

class Timer  : virtual public Channel
{
protected:
    int m_scale{};
    int m_period{};
    int m_periodClock{};

    Timer(int scale);

    word_t getFrequency();
    void setPeriod();
    bool updateTimer(int clocks);
};


class PulseChannel : public Envelope, public Timer
{
protected:
    int m_DutyCycle{};
    int m_DutyPointer{}; // indicates where we are along the duty cycle 

public:
    PulseChannel(MemoryBus& memoryRef, word_t r1, word_t r2, word_t r3, word_t r4);

    int generator() override;
    void update(int clocks) override;
    void trigger() override;

};

class SweepChannel : public PulseChannel
{
private:
    word_t m_NRx0{}; //
    int m_SweepPace{}; // updates on reset or retrigger
    int m_SweepCounter{};

    void setFrequency(word_t waveLen);
public:
    SweepChannel(MemoryBus& memoryRef, word_t r0, word_t r1, word_t r2, word_t r3, word_t r4);

    void trigger() override;
    void sweepIteration();
};

class WaveChannel : public Timer
{
private:
    word_t m_NRx0{}; //some optional channel specific feature

    int m_sampleIndex{};
    byte_t m_sampleBuffer{};
    int m_volumeLevel{};

public:
    WaveChannel(MemoryBus& memoryRef, 
        word_t r0, word_t r1, word_t r2, word_t r3, word_t r4);

    int generator() override;
    void update(int clocks) override;
    void trigger() override;

    void readNextWaveForm();
};

class NoiseChannel : public Envelope
{
public:
    NoiseChannel(MemoryBus& memoryRef, word_t r1, word_t r2, word_t r3, word_t r4);

    int generator() override;
    void update(int clocks) override;
    void trigger() override;
};