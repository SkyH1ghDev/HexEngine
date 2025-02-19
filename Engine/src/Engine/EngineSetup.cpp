#include <HexEngine/Engine/EngineSetup.hpp>
#include <HexEngine/Dev/SDLMemory.hpp>

std::shared_ptr<SDL_Window> EngineSetup::InitializeWindow()
{
    std::shared_ptr<SDL_Window> window(SDL_CreateWindow(
        "HexEngine",
        800,
        600,
        0
        ), SDL_DestroyWindow); 

    return window;
}
