#pragma once

#include <SDL2/SDL.h>

class Platform
{
private:
    SDL_Window* window{};
    SDL_Renderer* renderer{};
    SDL_Texture* texture{};

    int m_textureWidth{};
    int m_textureHeight{};

public:
    Platform(char const* title, int textureWidth, int textureHeight, int scale);
	~Platform();
	
	void Update(void const* buffer, int pitch);
	bool ProcessInput(uint8_t* keys);
};