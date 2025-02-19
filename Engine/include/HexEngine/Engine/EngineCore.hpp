#pragma once

#include <HexEngine/Renderer/Renderer.hpp>

class EngineCore
{
public:
    EngineCore() = default;
    ~EngineCore() = default;
    EngineCore(const EngineCore& other) = delete;
    EngineCore& operator=(const EngineCore& other) = delete;
    EngineCore(EngineCore&& other) noexcept = delete;
    EngineCore& operator=(EngineCore&& other) noexcept = delete;

    static void Run();

private:
    Renderer m_renderer;
};
