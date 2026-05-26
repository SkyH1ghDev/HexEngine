#include <PCH/EnginePCH.hpp>
#include "EngineSetup.hpp"

using namespace HexEngine::Engine;

HexEngine::SDL::SDLWindow EngineSetup::CreateWindow(const std::uint32_t pWidth, const std::uint32_t pHeight)
{
    HexEngine::SDL::SDLWindow window
    {
        SDL_CreateWindow("HexEngine", static_cast<int>(pWidth), static_cast<int>(pHeight), 0),
        pWidth,
        pHeight
    };

    return window;
}
