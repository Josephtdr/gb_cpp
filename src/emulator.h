#ifndef H_EMULATOR
#define H_EMULATOR

#include "consts.h"
#include "cpu.h"
#include "gpu.h"

class Emulator
{
private:
    CPU m_CPU{};
    GPU m_GPU{};


public:
    

    void frameUpdate();


};


#endif 