#include "EngineLoop.hpp"
#include "EngineSetup.hpp"
#include "EngineCore.hpp"

#include <HexEngine/Renderer/Renderer.hpp>
#include <HexEngine/Input/Input.hpp>
#include <SDL3/SDL_init.h>

bool EngineCore::m_shouldRun = true;

void EngineCore::Run()
{
    {
        Input::Initialize();
        Input::BindKey(SDL_SCANCODE_ESCAPE, PostQuitEvent_Callback);
        
        SDLWindow window(EngineSetup::InitializeWindow());
        Renderer renderer = Renderer(window);

        EngineLoop::Run(renderer);
    }

    Quit();
}

void EngineCore::Quit()
{
    SDL_Quit();
}

void EngineCore::PostQuitEvent_Callback(bool keyState)
{
    if (!keyState)
    {
        return;
    }
    
    SDL_Event event;
    SDL_zero(event);
    event.type = SDL_EVENT_QUIT;
    event.user.code = 0;
    event.user.data1 = nullptr;
    event.user.data2 = nullptr;
    SDL_PushEvent(&event);
}



