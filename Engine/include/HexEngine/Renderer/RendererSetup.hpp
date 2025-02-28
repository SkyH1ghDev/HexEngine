#pragma once

#include <dxgi1_6.h>
#include <memory>
#include <wrl/client.h>
#include <directx/d3dx12.h>
#include <SDL3/SDL_video.h>
#include <chrono>

namespace MW = Microsoft::WRL;

class RendererSetup
{
public:
    RendererSetup() = default;
    ~RendererSetup() = default;
    RendererSetup(const RendererSetup& other) = default;
    RendererSetup& operator=(const RendererSetup& other) = default;
    RendererSetup(RendererSetup&& other) noexcept = default;
    RendererSetup& operator=(RendererSetup&& other) noexcept = default;

    // DEBUG
    static MW::ComPtr<ID3D12Debug> CreateDebugLayer();
    
    static MW::ComPtr<ID3D12Device2> CreateDevice();
    static MW::ComPtr<ID3D12CommandQueue> CreateCommandQueue(const MW::ComPtr<ID3D12Device2>& device, const D3D12_COMMAND_LIST_TYPE& type);
    static MW::ComPtr<IDXGISwapChain4> CreateSwapChain(const std::shared_ptr<SDL_Window>& window, const MW::ComPtr<ID3D12CommandQueue>& commandQueue, const std::uint32_t& width, const std::uint32_t& height, const std::uint32_t& bufferCount);
    static MW::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(const MW::ComPtr<ID3D12Device2>& device, const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint32_t& numDescriptors);
    static std::vector<MW::ComPtr<ID3D12Resource>> CreateBackBuffers(const MW::ComPtr<ID3D12Device2>& device, const MW::ComPtr<IDXGISwapChain4>& swapChain, const MW::ComPtr<ID3D12DescriptorHeap>& descriptorHeap, const std::uint8_t& numBuffers);
    static MW::ComPtr<ID3D12CommandAllocator> CreateCommandAllocator(const MW::ComPtr<ID3D12Device2>& device, const D3D12_COMMAND_LIST_TYPE& type);
    static MW::ComPtr<ID3D12GraphicsCommandList> CreateCommandList(const MW::ComPtr<ID3D12Device2>& device, const MW::ComPtr<ID3D12CommandAllocator>& commandAllocator, const D3D12_COMMAND_LIST_TYPE& type);

    // Should Probably Be Moved Out Of Here
    static MW::ComPtr<ID3D12Fence> CreateFence(const MW::ComPtr<ID3D12Device2>& device);
    static HANDLE CreateFenceEvent();
    static std::uint64_t Signal(const MW::ComPtr<ID3D12CommandQueue>& commandQueue, const MW::ComPtr<ID3D12Fence>& fence, std::uint64_t& fenceValue);
    static void WaitForFenceValue(const MW::ComPtr<ID3D12Fence>& fence, const std::uint64_t& fenceValue, const HANDLE& fenceEvent, std::chrono::milliseconds duration = (std::chrono::milliseconds::max)());
    static void Flush(const MW::ComPtr<ID3D12CommandQueue>& commandQueue, const MW::ComPtr<ID3D12Fence>& fence, const HANDLE& fenceEvent, std::uint64_t& fenceValue);
    
private:
    static bool CheckTearingSupport();
};
