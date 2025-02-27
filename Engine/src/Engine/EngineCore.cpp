#include "EngineLoop.hpp"
#include "EngineSetup.hpp"
#include "EngineCore.hpp"

void EngineCore::Run()
{
    std::shared_ptr<SDL_Window> window = EngineSetup::InitializeWindow();
    Renderer renderer;
}
