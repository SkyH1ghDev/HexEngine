#pragma once
#include "ImGuiHeapAllocator.hpp"
#include <array>
#include <wrl/client.h>
#include <directx/d3dx12.h>
#include <HexEngine/SDL/SDLWindow.hpp>

namespace MW = Microsoft::WRL;

class ImGuiTool {
public:
    ImGuiTool() = default;
    ~ImGuiTool() = default;
    ImGuiTool(const ImGuiTool& other) = default;
    ImGuiTool& operator=(const ImGuiTool& other) = default;
    ImGuiTool(ImGuiTool&& other) noexcept = default;
    ImGuiTool& operator=(ImGuiTool&& other) noexcept = default;

    static void Initialize(const SDLWindow& window, const MW::ComPtr<ID3D12Device2>& device, const MW::ComPtr<ID3D12CommandQueue>& commandQueue, const MW::ComPtr<ID3D12DescriptorHeap>& srvDescriptorHeap, const DXGI_FORMAT& backBufferFormat, const UINT& maxFrameIndex);
    static void Start();
    static void Run();
    static void RenderDrawData(const MW::ComPtr<ID3D12GraphicsCommandList>& commandList);
    static void End();
    static void Shutdown();

private:
    static ImGuiHeapAllocator m_descriptorHeapAllocator;
    static std::array<std::uint8_t, 2> m_mouseButtons;
    
};
