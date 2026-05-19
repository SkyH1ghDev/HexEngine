#include "EngineSetup.hpp"

SDLWindow EngineSetup::InitializeWindow()
{
    SDLWindow window(SDL_CreateWindow(
        "HexEngine",
        1280,
        720,
        0
        ));

    return window;
}
