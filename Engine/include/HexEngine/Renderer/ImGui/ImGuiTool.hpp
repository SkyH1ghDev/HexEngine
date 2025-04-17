#pragma once
#include "ImGuiHeapAllocator.hpp"
#include <directx/d3dx12.h>
#include <HexEngine/SDL/SDLWindow.hpp>
#include <HexEngine/Renderer/DirectX/Device.hpp>
#include <HexEngine/Renderer/DirectX/CommandQueue.hpp>
#include <HexEngine/Renderer/DirectX/DescriptorHeap.hpp>
#include <HexEngine/Renderer/DirectX/CommandList.hpp>

class ImGuiTool {
public:
    ImGuiTool() = default;
    ~ImGuiTool() = default;
    ImGuiTool(const ImGuiTool& other) = default;
    ImGuiTool& operator=(const ImGuiTool& other) = default;
    ImGuiTool(ImGuiTool&& other) noexcept = default;
    ImGuiTool& operator=(ImGuiTool&& other) noexcept = default;

    static void Initialize(const SDLWindow& window, const Device& device, const CommandQueue& commandQueue, const DescriptorHeap& srvDescriptorHeap, DXGI_FORMAT backBufferFormat, std::uint64_t maxFrameIndex);
    static void Start();
    static void Run();
    static void RenderDrawData(const CommandList& commandList);
    static void End();
    static void Shutdown();

private:
    static ImGuiHeapAllocator m_descriptorHeapAllocator;
    
};
