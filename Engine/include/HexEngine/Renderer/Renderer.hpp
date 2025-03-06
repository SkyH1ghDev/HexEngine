#pragma once
#include <wrl/client.h>
#include <directx/d3dx12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <HexEngine/SDL/SDLWindow.hpp>
#include <HexEngine/Renderer/ImGui/ImGuiTool.hpp>

namespace MW = Microsoft::WRL;
namespace DX = DirectX;

class Renderer {
public:
    Renderer() = default;
    Renderer(const SDLWindow& window);
    ~Renderer();
    Renderer(const Renderer& other) = default;
    Renderer& operator=(const Renderer& other) = default;
    Renderer(Renderer&& other) noexcept = default;
    Renderer& operator=(Renderer&& other) noexcept = default;

    void Render() const;
    void Draw();

private:
    std::uint8_t m_numFrames = 3;
    bool m_VSync = false;

    // ImGui
    #if defined(_DEBUG)
    MW::ComPtr<ID3D12DescriptorHeap> m_imGuiSRVDescriptorHeap;
    #endif
    
    // DirectX12
    #ifdef _DEBUG
    MW::ComPtr<ID3D12Debug> m_debugInterface;
    #endif

    MW::ComPtr<ID3D12Device2> m_device;
    MW::ComPtr<ID3D12CommandQueue> m_commandQueue;
    MW::ComPtr<IDXGISwapChain4> m_swapChain;
    MW::ComPtr<ID3D12DescriptorHeap> m_backBufferDescriptorHeap;
    std::vector<MW::ComPtr<ID3D12Resource>> m_backBuffers;
    std::vector<MW::ComPtr<ID3D12CommandAllocator>> m_backBufferCommandAllocators;
    MW::ComPtr<ID3D12GraphicsCommandList> m_commandList;
    UINT m_rtvDescriptorSize;
    UINT m_backBufferIndex = 0;

    // Synchronization
    MW::ComPtr<ID3D12Fence> m_fence;
    std::uint64_t m_fenceValue = 0;
    std::vector<std::uint64_t> m_frameFenceValues = { 0, 0, 0 };
    HANDLE m_fenceEvent = nullptr;
};
