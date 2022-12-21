#include "inc/platform.h"
#include "inc/bitfuncs.h"

Platform::Platform(char const* title, int textureWidth, int textureHeight, int scale)
    : m_textureWidth{ textureWidth }, m_textureHeight{ textureHeight }
{   
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    window = SDL_CreateWindow(title, 0, 0, m_textureWidth*scale, m_textureHeight*scale, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, m_textureWidth, m_textureHeight);


    int SamplesPerSecond = 44100;
    int bufferSize = 735;


    SDL_AudioSpec AudioSettings = {0};
    AudioSettings.freq = SamplesPerSecond;
    AudioSettings.format = AUDIO_F32SYS;
    AudioSettings.channels = 2;
    AudioSettings.samples = bufferSize;

    SDL_OpenAudio(&AudioSettings, 0);
    

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

void Platform::updateAudio(void const* buffer, int len)
{
    SDL_QueueAudio(1, buffer, len * sizeof(float));
}

void Platform::pauseAudio()
{
    if (m_Playing)
    {   
        m_Playing = false;
        SDL_PauseAudio(1);
    }
}
void Platform::playAudio()
{
    if (!m_Playing)
    {
        m_Playing = true;
        SDL_PauseAudio(0);
    }
}

bool Platform::getExit() const
{
    return m_exit;
}

/**
 * @brief Processes keypresses, esc quits application.  
 * 
 * @return std::pair<int,int> first int [0-1] indicates arrowkey or button, second int [0-7] indicates specific key
 */
std::pair<int,int> Platform::ProcessInput()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
            {
                m_exit = true;
            } break;

            case SDL_KEYDOWN:
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE: m_exit = true; break;

                    case SDLK_DOWN:     return {0,3}; break;
                    case SDLK_UP:       return {0,2}; break;
                    case SDLK_LEFT:     return {0,1}; break;
                    case SDLK_RIGHT:    return {0,0}; break;

                    case SDLK_q:        return {0,7}; break;
                    case SDLK_e: return {0,6}; break;
                    case SDLK_d:        return {0,5}; break;
                    case SDLK_a:        return {0,4}; break;
                }
            } break;

            case SDL_KEYUP:
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_DOWN:     return {1,3}; break;
                    case SDLK_UP:       return {1,2}; break;
                    case SDLK_LEFT:     return {1,1}; break;
                    case SDLK_RIGHT:    return {1,0}; break;

                    case SDLK_q:        return {1,7}; break;
                    case SDLK_e: return {1,6}; break;
                    case SDLK_d:        return {1,5}; break;
                    case SDLK_a:        return {1,4}; break;
                }
            } break;
        }
    }
    return {2,0};
}