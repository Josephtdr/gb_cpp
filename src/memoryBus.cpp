#include "memoryBus.h"

#include <stdexcept> // for std::runtime_error

byte_t MemoryBus::readByte(word_t address) const
{
    if (0 < address || address >= c_MEMORY_SIZE)
    { 
        throw std::runtime_error("Trying to read from invalid memory address!");
    }


    return memory[address];
}

void MemoryBus::writeByte(word_t address, byte_t value)
{
    if (0 < address || address >= c_MEMORY_SIZE)
    { 
        throw std::runtime_error("Trying to write to invalid memory address!");
    }



    memory[address] = value; 
}
