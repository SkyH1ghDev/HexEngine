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
        EngineSetup(const EngineSetup& pOther) = default;
        EngineSetup& operator=(const EngineSetup& pOther) = default;
        EngineSetup(EngineSetup&& pOther) noexcept = default;
        EngineSetup& operator=(EngineSetup&& pOther) noexcept = default;

        // Init SDL3
        static HexEngine::SDL::SDLWindow CreateWindow(const std::uint32_t pWidth = 1280, const std::uint32_t pHeight = 720);
    };
}
