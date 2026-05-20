#include "EngineLoop.hpp"
#include "EngineSetup.hpp"
#include "EngineCore.hpp"

#include <HexEngine/Graphics/DirectX12/DX12Renderer.hpp>
#include <HexEngine/Input/InputHandler.hpp>
#include <SDL3/SDL_init.h>

#undef CreateWindow

using namespace HexEngine::Engine;

void EngineCore::Run()
{
    {
        HexEngine::Input::InputHandler::Initialize();
        HexEngine::Input::InputHandler::BindKey(SDL_SCANCODE_ESCAPE, PostQuitEvent_Callback);
        
        HexEngine::SDL::SDLWindow window {EngineSetup::CreateWindow()};
        HexEngine::Graphics::DirectX12::DX12Renderer renderer {window};

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



