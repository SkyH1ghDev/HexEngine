#pragma once

#include <SDL3/SDL.h>
#include <memory>

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
    static std::shared_ptr<SDL_Window> InitializeWindow();

    // Init DirectX 12
};
