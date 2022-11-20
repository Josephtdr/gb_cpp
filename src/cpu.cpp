#include "cpu.h"

void CPU::cycle()
{
    byte_t instructionByte = memory.readByte(pc);

    word_t nextPC = ((*this).*(instructionTable[instructionByte]))();  

    pc = nextPC;
}

