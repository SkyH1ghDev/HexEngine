#include "EngineSetup.hpp"

SDLWindow EngineSetup::InitializeWindow()
{
    SDLWindow window(SDL_CreateWindow(
        "HexEngine",
        800,
        600,
        0
        ));

    return window;
}
