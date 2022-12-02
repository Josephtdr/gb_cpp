#include "inc/cpu.h"



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
            drawScanLine();
        // we have entered vertical blank period
        else if (currentline == 144)
            requestInterupt(0);
        // if gone past max scanline, 153, reset
        else if (currentline > 153)
            m_Memory.writeByte(r_LY, 0);
    }
}

void CPU::drawScanLine()
{
    byte_t lcdControl{ readByte(r_LCDC) };
    if (testBit(lcdControl,0)) //background enabled flag
        //TODO: default to white background if bit 0 disabled
        renderTiles();
    if (testBit(lcdControl,1)) //sprites enabled flag
        return;
        renderSprites();
}


void CPU::renderTiles()
{
    byte_t lcdControl{ readByte(r_LCDC) };
    byte_t currentLine{ readByte(r_LY) };
    byte_t scrollY{ readByte(r_SCY) };
    byte_t scrollX{ readByte(r_SCX) };
    byte_t windowY{ readByte(r_WY) };
    byte_t windowX{ readByte(r_WX)-7 };

    bool drawingWindow{};
    bool signedTileLookup{};
    word_t tileDataAddress{};
    word_t backgroundTilemap{0x9800};
    word_t windowTilemap{0x9800};

    //Window enabled flag, and window visible on screen
    if (testBit(lcdControl, 5) && windowX <= 166 &&  windowY <= 143)
    {   
        if (windowY <= currentLine) //visible somewhere on current line
            drawingWindow = true;
    }

    if (testBit(lcdControl,4)) //Tile data flag
        tileDataAddress = 0x8000;
    else
    {
        tileDataAddress = 0x8800;
        signedTileLookup = true;
    }

    if (testBit(lcdControl, 3)) //Background memory flag
        backgroundTilemap  = 0x9C00;
    if (testBit(lcdControl, 6)) //Window memory flag
        windowTilemap  = 0x9C00;


    byte_t yPos{ currentLine + scrollY };

    for (int pixel{0}; pixel < 160; pixel++)
    {
        word_t tilemap{ backgroundTilemap };
        byte_t xPos{ pixel + scrollX };

        //currently pixel is a window tile
        if (drawingWindow && pixel >= windowX)
        {
            tilemap = windowTilemap;
            yPos = currentLine - windowY;
            xPos = pixel - windowX;
        }

        //which of the 8 verticle pixels within the tile
        word_t tileRow = (((byte_t)(yPos/8))*32);
        //which of the 8 horizontal pixels within the tile
        word_t tileCol = (xPos/8);
        word_t tileAddress = tilemap + tileRow + tileCol;

        word_t tileLocation{ getTileLocation(tileDataAddress, signedTileLookup, tileAddress) };
        
        int colourInt{ getColourInt(tileLocation, yPos, xPos) };
        Colour pixelColour{ getColour(colourInt, r_BG_PALLET) };

        int rgb{};
        // setup the RGB values
        switch(pixelColour)
        {
            case Colour::White: rgb = 255; break;
            case Colour::Light_Gray: rgb = 0xCC; break;
            case Colour::Dark_Gray: rgb = 0x77; break;
            case Colour::Black: rgb = 0; break;
        }

        if ((currentLine<0)||(currentLine>143)||(pixel<0)||(pixel>159))
        {
            throw std::runtime_error("Incorrect Drawing thingy!");
        }

        m_ScreenData[pixel][currentLine][0] = rgb;
        m_ScreenData[pixel][currentLine][1] = rgb;
        m_ScreenData[pixel][currentLine][2] = rgb;
    }
}

word_t CPU::getTileLocation(word_t tileDataAddress, bool signed_, word_t tileAddress)
{
    signed_word_t tileInt{};
    if (signed_)
        tileInt = static_cast<signed_byte_t>(readByte(tileAddress));
    else    
        tileInt = readByte(tileAddress);

    word_t tileLocation{ tileDataAddress };
    if (signed_)
        tileLocation += ((tileInt+128) * c_TILE_SIZE);
    else    
        tileLocation += (tileInt * c_TILE_SIZE);

    return tileLocation;
}

int CPU::getColourInt(word_t tileLocation, int yPos, int xPos)
{
    byte_t tileY{ (yPos % 8) * 2 }; //each line is two bytes long
    byte_t tileX{ -1*((xPos % 8) - 7) }; //bit 7 is pixel 0 etc...

    byte_t dataLow = readByte(tileLocation + tileY);
    byte_t dataHigh = readByte(tileLocation + tileY + 1);    

    int bit1{  testBit(dataHigh, tileX) };
    int bit0{  testBit(dataLow, tileX) };

    return (bit1 << 1) | bit0;
}

CPU::Colour CPU::getColour(int colourInt, word_t palletAddress)
{
    byte_t pallet{ readByte(palletAddress) };

    int transformedColour{ extractBits(pallet, colourInt*2,2) };

    switch(transformedColour)
    {
        case 0: return Colour::White;
        case 1: return Colour::Light_Gray;
        case 2: return Colour::Dark_Gray;
        case 3: return Colour::Black;
        default:
            throw std::runtime_error("Invalid Colour Type!");
    }
}


void CPU::updateLCDStatus()
{
    byte_t status = readByte(r_STAT); 

    if(!isLCDEnabled())
    {
        m_ScanlineCounter = 456;
        writeByte(r_LY, 0); 
        status &= 0xFC;
        setBit(status, 0);
        writeByte(r_STAT, status);
        return;
    }

    byte_t currentLine = readByte(r_LY);
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

    if (currentLine==readByte(r_LYC))
    {
        setBit(status,2); //LYC flag
        if (testBit(status,6)) //LYC interupt enabled flag
            requestInterupt(1);
    }
    else
        resetBit(status,2); //LYC flag

    writeByte(r_STAT, status);
}

bool CPU::isLCDEnabled()
{
    return testBit(readByte(r_LCDC),7);
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
        writeByte(0xFE00+i, readByte(startAddress+i));
    }
}




