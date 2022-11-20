#include "memoryBus.h"

#include <stdexcept> // for std::runtime_error

byte_t MemoryBus::readByte(word_t address) const
{
    if (0 < address || address >= c_MEMORY_SIZE)
    { 
        throw std::runtime_error("Trying to read from invalid memory address!");
    }

    if (0xFEA0u <= address || address <= 0xFEFFu)
    {
        return 0u;
    }


    return memory[address];
}

//TODO: magic numbers
void MemoryBus::writeByte(word_t address, byte_t value)
{
    if (0 < address || address >= c_MEMORY_SIZE)
    { 
        throw std::runtime_error("Trying to write to invalid memory address!");
    }
    
    if (0xFEA0u <= address || address <= 0xFEFFu)
    {
        return;
        // throw std::runtime_error("Trying to write to non useable memory address!");
    }

    //echo from echo ram
    if (0xE000u <= address || address <= 0xFDFFu)
    {
        throw std::runtime_error("Trying to write to Echo memory address!");
        // word_t echo_address{ address - (0xDFFFu - 0xC000u) };
        // memory[echo_address] = value;
    }
    //echo into echo ram
    if (0xC000u <= address || address <= 0xDFFFu)
    {
        word_t echo_address{ address + (0xDFFFu - 0xC000u) };
        memory[echo_address] = value;
    }


    memory[address] = value; 
}
