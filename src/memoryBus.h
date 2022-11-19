#ifndef H_MEMORYBUS
#define H_MEMORYBUS

#include "consts.h"

class MemoryBus
{
private:
    byte_t memory[c_MEMORY_SIZE]{};

public:
    byte_t readByte(word_t address) const;

    void writeByte(word_t address, byte_t value);

};


#endif