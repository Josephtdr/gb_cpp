#include "cpu.h"

word_t CPU::execute(byte_t instructionByte, bool prefixed)
{
    if (!prefixed)
        { return ((*this).*(instructionTable[instructionByte]))(); }
    else
        { return ((*this).*(prefixedInstructionTable[instructionByte]))(); }
}

void CPU::cycle()
{
    byte_t instructionByte = memory.readByte(pc);

    bool prefixed{ instructionByte == c_PREFIXED_INSTRUCTION_BYTE };
    if (prefixed)
        { instructionByte = memory.readByte(pc+1u); }

    word_t nextPC{ execute(instructionByte, prefixed) };

    pc = nextPC;
}

word_t CPU::jump(bool valid)
{   
    // Gameboy is little endian so read pc + 2 as most significant bit
    // and pc + 1 as least significant bit
    if (valid)
    {
        byte_t mostSignificantByte{ memory.readByte(pc+2u) };
        byte_t leastSignificantByte{ memory.readByte(pc+1u) };
        
        return (mostSignificantByte << 8) | leastSignificantByte;
    }
    else //skip the jump
    { 
        return (pc + 3u); 
    }
}