#pragma once
#include <wrl/client.h>
#include <directx/d3dx12.h>
#include <dxgi1_6.h>
#include <array>
#include <DirectXMath.h>

namespace MW = Microsoft::WRL;
namespace DX = DirectX;

class Renderer {
public:
    Renderer();
    ~Renderer() = default;
    Renderer(const Renderer& other) = default;
    Renderer& operator=(const Renderer& other) = default;
    Renderer(Renderer&& other) noexcept = default;
    Renderer& operator=(Renderer&& other) noexcept = default;

private:
    #ifdef _DEBUG
    MW::ComPtr<ID3D12Debug> m_debugInterface;
    #endif

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
