#pragma once
#include <DirectXMath.h>
#include <HexEngine/SDL/SDLWindow.hpp>
#include <HexEngine/Renderer/DirectX/DebugLayer.hpp>
#include <HexEngine/Renderer/DirectX/Device.hpp>
#include <HexEngine/Renderer/DirectX/SwapChainManager.hpp>
#include <HexEngine/Renderer/DirectX/CommandQueue.hpp>
#include <HexEngine/Renderer/DirectX/CommandList.hpp>

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

    void Render();
    void Draw();
    void Cleanup() const;

private:
    std::uint8_t m_numFrames = 3;
    bool m_vSync = false;

    // ImGui
    #if defined(_DEBUG)
        DebugLayer m_debugInterface;
        DescriptorHeap m_imGuiDescriptorHeap;
    #endif
    
    Device m_device;
    CommandQueue m_commandQueue;
    SwapChainManager m_swapChainManager;
    CommandList m_commandList;
    Fence m_fence;

    //winrt::com_ptr<ID3D12Device2> m_device;
    //MW::ComPtr<ID3D12CommandQueue> m_commandQueue;
    // MW::ComPtr<ID3D12DescriptorHeap> m_backBufferDescriptorHeap;
    // std::vector<MW::ComPtr<ID3D12Resource>> m_backBuffers;
    // std::vector<MW::ComPtr<ID3D12CommandAllocator>> m_backBufferCommandAllocators;
    //UINT m_backBufferIndex = 0;
    
    // Synchronization
    //MW::ComPtr<ID3D12Fence> m_fence;
    //std::uint64_t m_fenceValue = 0;
    //std::vector<std::uint64_t> m_frameFenceValues = { 0, 0, 0 };
    //HANDLE m_fenceEvent = nullptr;
};
