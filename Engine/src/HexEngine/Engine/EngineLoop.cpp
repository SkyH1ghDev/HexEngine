#include <PCH/EnginePCH.hpp>
#include "EngineLoop.hpp"

#include <HexEngine/Input/InputHandler.hpp>
#include <HexEngine/Time/Clock.hpp>
#include <HexEngine/SDL/SDLEventHandler.hpp>

using namespace HexEngine::Engine;

void EngineLoop::Run(HexEngine::Graphics::DirectX12::DX12Renderer& pRenderer)
{
    
    while (s_isRunning)
    {
        HexEngine::SDL::SDLEventHandler::HandleEvents();
        HexEngine::Input::InputHandler::Update();
        
        pRenderer.Draw();

        Time::Clock::Update();
    }
}

void EngineLoop::Stop()
{
    s_isRunning = false;
}
