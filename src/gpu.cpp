#include "inc/cpu.h"



const unsigned int c_VRAM_START = 0x8000;
const unsigned int c_VRAM_END = 0x97FF;





const int c_TILE_SIZE = 16; // Bytes !



void CPU::updateGraphics(int cycles)
{
    updateLCDStatus();

    if (isLCDEnabled())
        m_ScanlineCounter -= cycles;
    else
        return;
   
    if (m_ScanlineCounter <= 0)
    {
        // move onto next scanline
        m_Memory.increment(r_LY);
        byte_t currentline{ m_Memory.readByte(r_LY) };
        m_ScanlineCounter = 456; //number of cycles per scanline

        // draw the current scanline
        if (currentline < 144)
            return;
            // drawScanLine();
        // we have entered vertical blank period
        else if (currentline == 144)
            requestInterupt(0);
        // if gone past max scanline, 153, reset
        else if (currentline > 153)
            m_Memory.writeByte(r_LY, 0);
    }
}

void CPU::updateLCDStatus()
{
    byte_t status = m_Memory.readByte(r_STAT); 

    if(!isLCDEnabled())
    {
        m_ScanlineCounter = 456;
        m_Memory.writeByte(r_LY, 0); 
        status &= 0xFC;
        setBit(status, 0);
        m_Memory.writeByte(r_STAT, status);
        return;
    }

    byte_t currentLine = m_Memory.readByte(r_LY);
    byte_t currentMode = status & 0x3;
    byte_t newMode{};
    bool possibleInterupt{};


    if(currentLine < 144)
    {
        int mode2bounds = 456-80;
        int mode3bounds = mode2bounds - 172;
        
        if (m_ScanlineCounter >= mode2bounds) //Searching OAM
        {
            newMode = 2;
            setBit(status,1);
            resetBit(status,0);
            possibleInterupt = testBit(status, 5); //mode 2 interupt enabled flag
        }
        else if(m_ScanlineCounter >= mode3bounds) //Transferring Data to LCD Controller
        {
            newMode = 3;
            setBit(status,0);
            setBit(status,1);
        }
        else //HBlank
        {
            newMode = 0;
            resetBit(status,0);
            resetBit(status,1);
            possibleInterupt = testBit(status, 3); //mode 0 interupt enabled flag
        }
    }
    else //VBlank 
    {
        newMode = 1;
        setBit(status,0);
        resetBit(status,1);
        possibleInterupt = testBit(status, 4); //mode 1 interupt enabled flag
    }

    if (possibleInterupt && (currentMode!=newMode))
    {
        requestInterupt(1);
    }

    if (currentLine==m_Memory.readByte(r_LYC))
    {
        setBit(status,2); //LYC flag
        if (testBit(status,6)) //LYC interupt enabled flag
            requestInterupt(1);
    }
    else
        resetBit(status,2); //LYC flag

    m_Memory.writeByte(r_STAT, status);
}

bool CPU::isLCDEnabled()
{
    return testBit(m_Memory.readByte(r_LCDC),7);
}

/**
 * @brief Performs the Direct Memory Access transfer, which maps the sprite data 
 * at address 'value' * 100, to the sprite ram at (0xFE00-0xFE9F).
 */
void CPU::initiateDMATransfer(byte_t value)
{
    word_t startAddress{ static_cast<word_t>(value << 8) };
    for (int i{0}; i<0xA0; i++)
    {
        m_Memory.writeByte(0xFE00+i, m_Memory.readByte(startAddress+i));
    }
}



void CPU::updateAddressingMode()
{
    // m_Memory.readByte();
}




