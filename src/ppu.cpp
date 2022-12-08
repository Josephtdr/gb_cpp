#include "inc/ppu.h"
#include "inc/bitfuncs.h"

const int c_CYCLES_PER_SCANLINE {456};
const int c_MAX_SCANLINE {153};

PPU::PPU(MemoryBus& memoryRef, logger& logRef, Platform& platformRef)
    : m_Memory{ memoryRef },
      m_log{ logRef },
      m_Platform{ platformRef }
{
    m_Platform.Update(m_textureBuffer, c_VIDEO_WIDTH * sizeof(uint32_t));
}

void PPU::updateGraphics(int cycles)
{
    updateLCDStatus();

    if (!isLCDEnabled())
        return;
    
    m_ScanlineCounter -= cycles;

    if (m_ScanlineCounter <= 0)
    {
        m_ScanlineCounter = c_CYCLES_PER_SCANLINE;
        byte_t currentline{ readByte(r_LY) };
        m_Memory.increment(r_LY);
        // draw the current scanline
        if (currentline < c_VIDEO_HEIGHT)
            drawScanLine(currentline);
        // Perform interupt when entering VBlank
        else if (currentline == c_VIDEO_HEIGHT)
            requestInterupt(0);
        // if gone past max scanline, 153, reset
        else if (currentline > c_MAX_SCANLINE)
            writeByte(r_LY, 0);
    }
}

void PPU::renderScreen()
{
    for (byte_t line{0}; line<c_VIDEO_HEIGHT; ++line)
    {
        for (byte_t pixel{0}; pixel<c_VIDEO_WIDTH; ++pixel)
        {
            uint32_t rgb{};
            switch(m_ScreenData[pixel][line].colour)
            {
                case Colour::White: rgb = 0xFFFFFFFF; break;
                case Colour::Light_Gray: rgb = 0xAAAAAAFF; break;
                case Colour::Dark_Gray: rgb = 0x555555FF; break;
                case Colour::Black: rgb = 0x000000FF; break;
                // default:
                //     throw std::logic_error("Invalid pixel colour!");
            }
            m_textureBuffer[line*c_VIDEO_WIDTH + pixel] = rgb;
        }
    }
    m_Platform.Update(m_textureBuffer, c_VIDEO_WIDTH * sizeof(uint32_t));
}

void PPU::drawScanLine(int line)
{
    byte_t lcdControl{ readByte(r_LCDC) };
    if (testBit(lcdControl,0)) //background enabled flag
        renderTiles(line);
    else
        renderWhite(line);
    if (testBit(lcdControl,1)) //sprites enabled flag
        renderSprites(line);
}

void PPU::renderWhite(int currentLine)
{
    for (byte_t pixel{0}; pixel<c_VIDEO_WIDTH; ++pixel)
    {
        m_ScreenData[pixel][currentLine] = Pixel{ Colour::White, false, true};
    }
}

void PPU::renderTiles(int currentLine)
{
    byte_t lcdControl{ readByte(r_LCDC) };
    byte_t scrollY{ readByte(r_SCY) };
    byte_t scrollX{ readByte(r_SCX) };
    byte_t windowY{ readByte(r_WY) };
    byte_t windowX{ static_cast<byte_t>(readByte(r_WX)-7) };

    bool drawingWindow{};
    bool signedTileLookup{};
    word_t tileDataAddress{};
    word_t backgroundTilemap{0x9800};
    word_t windowTilemap{0x9800};

    //Window enabled flag, and window visible on screen
    if (testBit(lcdControl, 5) && windowX < c_VIDEO_WIDTH+7 &&  windowY < c_VIDEO_HEIGHT)
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


    byte_t yPos{ static_cast<byte_t>(currentLine + scrollY) };

    for (int pixel{0}; pixel < c_VIDEO_WIDTH; ++pixel)
    {
        bool inWindow{};
        word_t tilemap{ backgroundTilemap };
        byte_t xPos{ static_cast<byte_t>(pixel + scrollX) };

        //currently pixel is a window tile
        if (drawingWindow && pixel >= windowX)
        {
            tilemap = windowTilemap;
            yPos = currentLine - windowY;
            xPos = pixel - windowX;
            inWindow = true;
        }

        //which of the 8 verticle pixels within the tile
        word_t tileMapRow = (((byte_t)(yPos/8))*32);
        //which of the 8 horizontal pixels within the tile
        word_t tileMapCol = (xPos/8);
        word_t tileAddress = tilemap + tileMapRow + tileMapCol;

        word_t tileLocation{ getTileLocation(tileDataAddress, signedTileLookup, tileAddress) };
        
        byte_t tileY{ static_cast<byte_t>(yPos % 8) };
        byte_t tileX{ static_cast<byte_t>(-1*((xPos % 8) - 7)) }; //bit 7 is pixel 0 etc...
        
        int colourInt{ getColourInt(tileLocation, tileY, tileX) };
        Colour pixelColour{ getColour(colourInt, r_BG_PALLET) };

        m_ScreenData[pixel][currentLine] = Pixel{ pixelColour, false, colourInt==0 };
    }
}

word_t PPU::getTileLocation(word_t tileDataAddress, bool signed_, word_t tileAddress)
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

void PPU::renderSprites(int currentLine)
{
    byte_t lcdControl{ readByte(r_LCDC) };
    word_t tileDataAddress{0x8000};
    int height {testBit(lcdControl, 2) ? 16 : 8};

    std::vector<Sprite> sprites{10};
    getSprites(sprites, currentLine, height);

    for(const auto& sprite : sprites)
    {
        word_t pallet{ testBit(sprite.flags, 4) ? r_SPRITE_PALLET2 : r_SPRITE_PALLET1 };
        word_t tileLocation = tileDataAddress + (sprite.tileIndex * c_TILE_SIZE);

        byte_t tileY{ static_cast<byte_t>(currentLine - (sprite.yPos-16)) };
        if (testBit(sprite.flags, 6)) //y flip flag
            tileY = -1*((tileY % height) - (height-1));

        if (height==16)
        {
            if (tileY < 8)
                tileLocation = tileLocation & 0xFE;
            else
            {
                tileLocation = tileLocation | 0x1;
                tileY-= 8;
            }
        }

        for (int pixel {0}; pixel < 8; ++pixel)
        {
            byte_t tileX {static_cast<byte_t>(-1*((pixel % 8) - 7))}; //bit 7 is pixel 0 etc...
            if (testBit(sprite.flags, 5)) //x flip flag
                tileX = -1*((tileX % 8) - 7);

            int pixelX {sprite.xPos - 8 + pixel};
            //pixel not on screen
            if (pixelX<0 || pixelX>c_VIDEO_WIDTH)
                continue;

            int colourInt {getColourInt(tileLocation, tileY, tileX)};
            Colour colour{ getColour(colourInt, pallet, true) };
            Pixel& screenPixel{ m_ScreenData[pixelX][currentLine] };

            //dont draw over previous sprites
            if (screenPixel.sprite)
                continue;
            //dont draw if both priorities set to bg 
            if (testBit(lcdControl, 0) && testBit(sprite.flags, 7))
            {
                if (!screenPixel.transparent) // and bg not transparent
                    continue;
            }
            if (colour==Colour::Transparent) //dont draw if transparent
                continue;

            //finally can draw
            screenPixel = Pixel{ colour, true, false };
        }
    }
}

void PPU::getSprites(std::vector<Sprite>& sprites, byte_t LY, int height)
{
    word_t OAMTable{0xFE00};
    int numSprites{};

    for (byte_t i {0}; i < 40; ++i)
    {
        if (numSprites>=10)
            continue;
        
        byte_t yPos{ readByte(OAMTable + i*4) };
        if (LY < (yPos-16) || LY >= (yPos-16+height))
            continue;

        byte_t xPos{ readByte(OAMTable + i*4 + 1u) };
        byte_t tileIndex{ readByte(OAMTable + i*4 + 2u) };
        byte_t flags{ readByte(OAMTable + i*4 + 3u) };
        
        sprites[numSprites] = Sprite{ yPos, xPos, tileIndex, flags };
        ++numSprites;
    }
}

int PPU::getColourInt(word_t tileLocation, int tileY, int tileX)
{
    byte_t dataLow {readByte(tileLocation + (tileY * 2))};
    byte_t dataHigh {readByte(tileLocation + (tileY * 2) + 1)};    

    int bit1 {testBit(dataHigh, tileX)};
    int bit0 {testBit(dataLow, tileX)};

    return (bit1 << 1) | bit0;
}

PPU::Colour PPU::getColour(int colourInt, word_t palletAddress, bool obj)
{
    byte_t pallet{ readByte(palletAddress) };

    int transformedColour{ extractBits(pallet, colourInt*2,2) };

    switch(transformedColour)
    {
        case 0: return (obj ? Colour::Transparent : Colour::White); break;
        case 1: return Colour::Light_Gray; break;
        case 2: return Colour::Dark_Gray; break;
        case 3: return Colour::Black; break;
        default:
            throw std::runtime_error("Invalid Colour Type!");
    }
}


void PPU::updateLCDStatus()
{
    byte_t status = readByte(r_STAT); 

    if(!isLCDEnabled())
    {
        m_ScanlineCounter = c_CYCLES_PER_SCANLINE;
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

    if(currentLine < c_VIDEO_HEIGHT)
    {
        int mode2bounds = c_CYCLES_PER_SCANLINE-80;
        int mode3bounds = mode2bounds - 172;
        
        if (m_ScanlineCounter >= mode2bounds) //Searching OAM
        {
            newMode = 2;
            resetBit(status,0);
            setBit(status,1);
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

    //lcd interupt
    if (possibleInterupt && (currentMode!=newMode))
        requestInterupt(1);

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

bool PPU::isLCDEnabled()
{
    return testBit(readByte(r_LCDC),7);
}

/**
 * @brief 0 Vblank, 1 LCD, 2 Timer, 4 Joypad input
 */
void PPU::requestInterupt(int interupt) //0,1,2,4
{
    byte_t requests = readByte(r_IF);
    //set relevent bit
    setBit(requests, interupt);
    writeByte(r_IF, requests);
}

byte_t PPU::readByte(word_t address) const
{
    return m_Memory.readByte(address);
}
void PPU::writeByte(word_t address, byte_t value)
{
    //reset the current scanline if the game tries to write to it
    if (address == r_LY)
    {
        m_Memory.writeByte(r_LY, 0);
        m_ScanlineCounter = c_CYCLES_PER_SCANLINE; //number of cycles per scanline
    }
    m_Memory.writeByte(address, value);
}