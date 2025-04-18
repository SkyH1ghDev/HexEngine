#include "EngineLoop.hpp"

#include <HexEngine/Input/Input.hpp>
#include <HexEngine/Clock/Clock.hpp>
#include <HexEngine/SDL/SDLEventHandler.hpp>

bool EngineLoop::m_isRunning = true;

void EngineLoop::Run(Renderer& renderer)
{
    
    while (m_isRunning)
    {
        Input::Update();
        
        renderer.Draw();
        
        SDLEventHandler::HandleEvents();
        Clock::Update();
    }

    renderer.Cleanup();
}

void EngineLoop::Stop()
{
    m_isRunning = false;
}
