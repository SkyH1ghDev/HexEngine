#pragma once
#include "ImGuiHeapAllocator.hpp"
#include <directx/d3dx12.h>
#include <HexEngine/SDL/SDLWindow.hpp>
#include <HexEngine/Graphics/DirectX12/Device.hpp>
#include <HexEngine/Graphics/DirectX12/CommandQueue.hpp>
#include <HexEngine/Graphics/DirectX12/DescriptorHeap.hpp>
#include <HexEngine/Graphics/DirectX12/CommandList.hpp>

namespace HexEngine::Graphics::UI
{
    class ImGuiTool {
    public:
        ImGuiTool() = default;
        ~ImGuiTool() = default;
        ImGuiTool(const ImGuiTool& other) = default;
        ImGuiTool& operator=(const ImGuiTool& other) = default;
        ImGuiTool(ImGuiTool&& other) noexcept = default;
        ImGuiTool& operator=(ImGuiTool&& other) noexcept = default;

        static void Initialize(const HexEngine::SDL::SDLWindow& window, const Device& device, const CommandQueue& commandQueue, const DescriptorHeap& srvDescriptorHeap, DXGI_FORMAT backBufferFormat, std::uint64_t maxFrameIndex);
        static void Start();
        static void Run();
        static void RenderDrawData(const CommandList& commandList);
        static void End();
        static void Shutdown();

    private:
        inline static ImGuiHeapAllocator s_descriptorHeapAllocator {};
    
    };
}
