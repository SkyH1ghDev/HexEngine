#pragma once
#include <wrl/client.h>
#include <d3d12.h>
#include <dxgi.h>
#include <array>

namespace MW = Microsoft::WRL;
// namespace DX = DirectX;

class Renderer {
public:
    Renderer() = default;
    ~Renderer() = default;
    Renderer(const Renderer& other) = default;
    Renderer& operator=(const Renderer& other) = default;
    Renderer(Renderer&& other) noexcept = default;
    Renderer& operator=(Renderer&& other) noexcept = default;

    void Setup();

private:
    void EnableDebugLayer();
    
private:
    MW::ComPtr<ID3D12Device> m_device;
    MW::ComPtr<ID3D12CommandQueue> m_commandQueue;
    MW::ComPtr<ID3D12CommandAllocator> m_commandAllocator;
    MW::ComPtr<ID3D12GraphicsCommandList> m_commandList;
    MW::ComPtr<IDXGISwapChain> m_swapChain;
    std::array<MW::ComPtr<ID3D12Resource>, 3> m_backBuffer;
    MW::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
    UINT m_rtvDescriptorSize;
    UINT m_backBufferIndex;
};
