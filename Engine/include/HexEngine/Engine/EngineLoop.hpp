#pragma once
#include <HexEngine/Graphics/DirectX12/DX12Renderer.hpp>

namespace HexEngine::Engine
{
    class EngineLoop
    {
    public:
        EngineLoop() = default;
        ~EngineLoop() = default;
        EngineLoop(const EngineLoop& other) = default;
        EngineLoop& operator=(const EngineLoop& other) = default;
        EngineLoop(EngineLoop&& other) noexcept = default;
        EngineLoop& operator=(EngineLoop&& other) noexcept = default;

        static void Run(HexEngine::Graphics::DirectX12::DX12Renderer& renderer);
        static void Stop();
    
    private:
        inline static bool s_isRunning = true;
    };
}
