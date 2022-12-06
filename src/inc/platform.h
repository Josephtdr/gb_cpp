#pragma once

#include <SDL2/SDL.h>
#include "consts.h"

class Platform
{
private:
    SDL_Window* window{};
    SDL_Renderer* renderer{};
    SDL_Texture* texture{};

    int m_textureWidth{};
    int m_textureHeight{};
    int m_exit{};

public:
    Platform(char const* title, int textureWidth, int textureHeight, int scale);
	~Platform();
	
	void Update(void const* buffer, int pitch);
	std::pair<int,int> ProcessInput();
    bool getExit() const;
};