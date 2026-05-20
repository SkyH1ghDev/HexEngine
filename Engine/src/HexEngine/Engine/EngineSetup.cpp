#include "EngineSetup.hpp"

using namespace HexEngine::Engine;

HexEngine::SDL::SDLWindow EngineSetup::CreateWindow()
{
    HexEngine::SDL::SDLWindow window(SDL_CreateWindow(
        "HexEngine",
        1280,
        720,
        0
        ));

    return window;
}
