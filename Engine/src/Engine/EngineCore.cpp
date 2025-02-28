#include "EngineLoop.hpp"
#include "EngineSetup.hpp"
#include "EngineCore.hpp"
#include <HexEngine/Clock/Clock.hpp>
#include <HexEngine/Renderer/Renderer.hpp>

void EngineCore::Run()
{
    std::shared_ptr<SDL_Window> window = EngineSetup::InitializeWindow();
    Renderer renderer = Renderer(window);

    while (true)
    {
        Clock::Update();
    }
}
