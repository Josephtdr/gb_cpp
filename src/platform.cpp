#include "inc/platform.h"
#include "inc/bitfuncs.h"

Platform::Platform(char const* title, int textureWidth, int textureHeight, int scale)
    : m_textureWidth{ textureWidth }, m_textureHeight{ textureHeight }
{   
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(title, 0, 0, m_textureWidth*scale, m_textureHeight*scale, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, m_textureWidth, m_textureHeight);
}

Platform::~Platform()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Platform::Update(void const* buffer, int pitch)
{
    SDL_UpdateTexture(texture, nullptr, buffer, pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

bool Platform::ProcessInput(byte_t& keys)
{
    bool quit = false;
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
            {
                quit = true;
            } break;

            case SDL_KEYDOWN:
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE: quit = true; break;

                    case SDLK_DOWN:     resetBit(keys,3); break;
                    case SDLK_UP:       resetBit(keys,2); break;
                    case SDLK_LEFT:     resetBit(keys,1); break;
                    case SDLK_RIGHT:    resetBit(keys,0); break;

                    case SDLK_q:        resetBit(keys,7); break;
                    case SDLK_KP_ENTER: resetBit(keys,6); break;
                    case SDLK_d:        resetBit(keys,5); break;
                    case SDLK_a:        resetBit(keys,4); break;
                }
            } break;

            case SDL_KEYUP:
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_DOWN:     setBit(keys,3); break;
                    case SDLK_UP:       setBit(keys,2); break;
                    case SDLK_LEFT:     setBit(keys,1); break;
                    case SDLK_RIGHT:    setBit(keys,0); break;

                    case SDLK_q:        setBit(keys,7); break;
                    case SDLK_KP_ENTER: setBit(keys,6); break;
                    case SDLK_d:        setBit(keys,5); break;
                    case SDLK_a:        setBit(keys,4); break;
                }
            } break;
        }
    }
    return quit;
}