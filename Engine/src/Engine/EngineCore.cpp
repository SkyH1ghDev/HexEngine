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


