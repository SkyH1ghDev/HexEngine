#include "EngineLoop.hpp"

#include <HexEngine/Input/Input.hpp>
#include <HexEngine/Clock/Clock.hpp>
#include <HexEngine/SDL/SDLEventHandler.hpp>

bool EngineLoop::m_isRunning = true;

void EngineLoop::Run(Renderer& renderer)
{
    Input::BindKey(SDL_SCANCODE_ESCAPE, Stop);
    Input::UnbindKey(SDL_SCANCODE_ESCAPE, Stop);
    
    while (m_isRunning)
    {
        renderer.Draw();
        
        SDLEventHandler::HandleEvents();
        Clock::Update();
        Input::Update();
    }
}

void EngineLoop::Stop()
{
    m_isRunning = false;
}
