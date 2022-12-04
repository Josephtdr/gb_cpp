#pragma once

#include "consts.h"
#include "BSlogger.h"
#include "memoryBus.h"

#include <vector>

const int c_TILE_SIZE = 16; // Bytes !
const int c_VIDEO_WIDTH = 160;
const int c_VIDEO_HEIGHT = 144;

class PPU
{
private:
    enum class Colour
    {
        Transparent, White, Light_Gray, Dark_Gray, Black
    };
    struct Sprite
    {
        byte_t yPos{};
        byte_t xPos{};
        byte_t tileIndex{};
        byte_t flags{};
    };
    struct Pixel
    {
        Colour colour{};
        bool sprite{};
        bool transparent{};
    };
    Pixel m_ScreenData[c_VIDEO_WIDTH][c_VIDEO_HEIGHT]{};
    uint32_t video[c_VIDEO_WIDTH * c_VIDEO_HEIGHT]{};
    int m_ScanlineCounter{}; //Tracks when to increment current scanline
    MemoryBus& m_Memory;
    logger& m_log;

public:
    PPU(MemoryBus& memoryRef, logger& logRef);

    void renderScreen();
    void updateGraphics(int cycles);

private:
    void updateLCDStatus();
    bool isLCDEnabled();

    void drawScanLine();
    void renderTiles();
    void renderWhite();
    word_t getTileLocation(word_t tileDataBase, bool signed_, word_t tileAddress);
    
    void renderSprites();
    void getSprites(std::vector<Sprite>& sprites, byte_t LY, int height);

    int getColourInt(word_t tileLocation, int yPos, int xPos);
    Colour getColour(int colourInt, word_t palletAddress, bool obj=false);

    byte_t readByte(word_t address) const;
    void writeByte(word_t address, byte_t value);
    void requestInterupt(int interupt); //duplicated to cpu version
};