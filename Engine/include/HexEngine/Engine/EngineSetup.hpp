#pragma once

#include <HexEngine/SDL/SDLWindow.hpp>

#undef CreateWindow

namespace HexEngine::Engine
{
    class EngineSetup
    {
    public:
        EngineSetup() = default;
        ~EngineSetup() = default;
        EngineSetup(const EngineSetup& other) = default;
        EngineSetup& operator=(const EngineSetup& other) = default;
        EngineSetup(EngineSetup&& other) noexcept = default;
        EngineSetup& operator=(EngineSetup&& other) noexcept = default;

        // Init SDL3
        static HexEngine::SDL::SDLWindow CreateWindow();
    };
}
