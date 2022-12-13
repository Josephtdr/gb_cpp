#pragma once

#include "consts.h"
#include "memoryBus.h"

class Channel
{
protected:
    MemoryBus& m_Memory;
    word_t m_NRx1{}; 
    word_t m_NRx2{}; 
    word_t m_NRx3{}; 
    word_t m_NRx4{}; 
    int m_InitialLengthTimer{};
    int m_baseLenTimer{};
    int m_lenBitSize{};
    bool m_ChannelEnabled{};
    bool m_DacEnabled{};

    Channel(MemoryBus& memoryRef, 
        word_t r1, word_t r2, word_t r3, word_t r4, 
        int baseLenTimer=64, int lenBitSize=6);
public: 
    bool channelEnabled() const;
    bool dacEnabled() const;
    void incrementLenTimer();

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

class Frequency  : virtual public Channel
{
protected:
    int m_scale{};
    int m_period{};

    Frequency(int scale);

    word_t getFrequency();
    int updatePeriod();
};


class PulseChannel : public Envelope, public Frequency
{
protected:
    int m_DutyCycle{};
    int m_DutyCounter{};

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

    void setWaveLen(word_t waveLen);
public:
    SweepChannel(MemoryBus& memoryRef, word_t r0, word_t r1, word_t r2, word_t r3, word_t r4);

    void trigger() override;
    void update(int clocks) override;
    void sweepIteration();
};

class WaveChannel : public Frequency
{
private:
    word_t m_NRx0{}; //some optional channel specific feature

    int m_sampleIndex{};
    byte_t m_sampleBuffer{};

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

//sweep+pulse
const word_t r_NR10 = 0xFF10; //sweep
const word_t r_NR11 = 0xFF11; //length timer & duty cycle
const word_t r_NR12 = 0xFF12; //volume & envelope
const word_t r_NR13 = 0xFF13; //wavelength low 
const word_t r_NR14 = 0xFF14; //wavelength high (0-2) & control
//pulse
const word_t r_NR21 = 0xFF16;
const word_t r_NR22 = 0xFF17;
const word_t r_NR23 = 0xFF18;
const word_t r_NR24 = 0xFF19;
//wave
const word_t r_NR30 = 0xFF1A; //dac enable
//TODO: m_memory fucntion, write to here enables disables this dac + channel
const word_t r_NR31 = 0xFF1B; //length timer
const word_t r_NR32 = 0xFF1C; //volume 
const word_t r_NR33 = 0xFF1D; //wavelength low 
const word_t r_NR34 = 0xFF1E; //wavelength high (0-2) & control
//noise
const word_t r_NR41 = 0xFF20; //length timer
const word_t r_NR42 = 0xFF21; //volume & envelope
const word_t r_NR43 = 0xFF22; //frequency & randomness 
const word_t r_NR44 = 0xFF23; //control