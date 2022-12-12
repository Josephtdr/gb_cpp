#pragma once

#include "consts.h"
#include "memoryBus.h"

class Channel
{
protected:
    MemoryBus& m_Memory;
    bool m_ChannelEnabled{};
    bool m_DacEnabled{};

    Channel(MemoryBus& memoryRef);

public: 
    bool channelEnabled() const;
    bool dacEnabled() const;

    virtual int generator() = 0;
    virtual void update(int clocks) = 0;
    virtual void trigger() = 0;
    virtual void incrementLenTimer() = 0;

    virtual ~Channel() = default;

protected:
    virtual byte_t getVolume() const = 0;
    virtual word_t getWaveLen() const = 0;
    virtual void setWaveLen(word_t waveLen) = 0; 
};

class Channel_1 : public Channel
{
private:
    int m_DutyStep{};
    int m_InitialLengthTimer{};

    int m_SweepPace{}; // updates on reset or retrigger
    int m_EnvelopeSweepPace{}; // updates only on reset
    bool m_EnvelopeIncrease{}; // false decrease, updates only on reset
    int m_InitialEnvelopeVolume{}; // updates only on reset
    
public:
    Channel_1(MemoryBus& memoryRef);

    int generator() override;
    void trigger() override;
    void update(int clocks) override;
    void sweepIteration();
    void incrementLenTimer() override;

private:
    byte_t getVolume() const override;
    word_t getWaveLen() const override;
    void setWaveLen(word_t waveLen) override;
};