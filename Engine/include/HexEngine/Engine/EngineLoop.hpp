#pragma once
#include <HexEngine/Renderer/Renderer.hpp>

class EngineLoop
{
public:
    EngineLoop() = default;
    ~EngineLoop() = default;
    EngineLoop(const EngineLoop& other) = default;
    EngineLoop& operator=(const EngineLoop& other) = default;
    EngineLoop(EngineLoop&& other) noexcept = default;
    EngineLoop& operator=(EngineLoop&& other) noexcept = default;

    static void Run(Renderer& renderer);
    static void Stop();
    
private:
    static bool m_isRunning;
};
