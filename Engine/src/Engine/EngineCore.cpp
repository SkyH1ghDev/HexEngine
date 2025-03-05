#include "EngineLoop.hpp"
#include "EngineSetup.hpp"
#include "EngineCore.hpp"
#include <HexEngine/Clock/Clock.hpp>
#include <HexEngine/Renderer/Renderer.hpp>
#include <HexEngine/SDL/SDLEventHandler.hpp>
#include <SDL3/SDL_init.h>

#include "Input.hpp"

bool EngineCore::m_shouldRun = true;

void EngineCore::Run()
{
    {
        Input::Initialize();
    
        SDLWindow window(EngineSetup::InitializeWindow());
        Renderer renderer = Renderer(window);


        // TODO: Move to EngineLoop
        while (m_shouldRun)
        {
            SDLEventHandler::HandleEvents();
        
            // renderer.Draw();
            Clock::Update();
        }
    }

    Quit();
}

void EngineCore::Quit()
{
    SDL_Quit();
}

// TODO: Move to EngineLoop
void EngineCore::QuitCallback()
{
    m_shouldRun = false;
}
