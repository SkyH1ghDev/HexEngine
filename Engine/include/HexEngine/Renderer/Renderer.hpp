#pragma once
#include <wrl/client.h>
#include <directx/d3dx12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <SDL3/SDL.h>

namespace MW = Microsoft::WRL;
namespace DX = DirectX;

class Renderer {
public:
    Renderer() = default;
    Renderer(const std::shared_ptr<SDL_Window>& window);
    ~Renderer() = default;
    Renderer(const Renderer& other) = default;
    Renderer& operator=(const Renderer& other) = default;
    Renderer(Renderer&& other) noexcept = default;
    Renderer& operator=(Renderer&& other) noexcept = default;

    void Render();

private:
    std::uint8_t g_NumFrames = 3;
    
    #ifdef _DEBUG
    MW::ComPtr<ID3D12Debug> m_debugInterface;
    #endif

    MW::ComPtr<ID3D12Device2> m_device;
    MW::ComPtr<ID3D12CommandQueue> m_commandQueue;
    MW::ComPtr<IDXGISwapChain4> m_swapChain;
    MW::ComPtr<ID3D12DescriptorHeap> m_backBufferDescriptorHeap;
    std::vector<MW::ComPtr<ID3D12Resource>> m_backBuffers;
    MW::ComPtr<ID3D12CommandAllocator> m_commandAllocator;
    MW::ComPtr<ID3D12GraphicsCommandList> m_commandList;
    MW::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
    UINT m_rtvDescriptorSize;
    UINT m_backBufferIndex;
};
