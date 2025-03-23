#include "EngineLoop.hpp"

#include <HexEngine/Input/Input.hpp>
#include <HexEngine/Clock/Clock.hpp>
#include <HexEngine/SDL/SDLEventHandler.hpp>

bool EngineLoop::m_isRunning = true;

void EngineLoop::Run(Renderer& renderer)
{
    Input::BindKey(SDL_SCANCODE_ESCAPE, Stop);
    
    while (m_isRunning)
    {
        Input::Update();
        
        renderer.Draw();
        
        SDLEventHandler::HandleEvents();
        Clock::Update();
    }

    renderer.Cleanup();
}

void EngineLoop::Stop(const bool& keyState)
{
    bool isRunning = !keyState;
    m_isRunning = isRunning;
}
