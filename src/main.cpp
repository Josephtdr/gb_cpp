#include <iostream>

#include "registers.h"



int main()
{
    Registers reg{};

    std::cout<< "yo! ";
    reg.b = 0xA1;
    reg.c = 0xB1;

    reg.set_af(0xf16c);

    std::cout << std::hex << reg.get_af() << "\n";

    reg.set_af(0xf16c);

    //+reg.c causes integer promotion, allowing proper printing
    std::cout << "a:" << static_cast<int>(reg.a) << ", f:" << +reg.f << "\n";

    // byte_t byte{ 0b11101111 };

    // reg.f = byte;

    // std::cout << "byte: " << +byte << " reg.f:" << +reg.f << "\n";

    std::cout << std::boolalpha << reg.f.zero << reg.f.subtract << reg.f.half_carry << reg.f.carry << "\n";
}