#include "cpu.h"

int CPU::OP_OxF1()
{
    registers.set_af( pop() );
    return 12;
} //POP AF
int CPU::OP_OxC1()
{
    registers.set_bc( pop() );
    return 12;
} //POP BC
int CPU::OP_OxD1()
{
    registers.set_de( pop() );
    return 12;
} //POP DE
int CPU::OP_OxE1()
{
    registers.set_hl( pop() );
    return 12;
} //POP HL
