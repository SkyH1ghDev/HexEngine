#pragma once

#include <dxgi1_6.h>
#include <directx/d3dx12.h>
#include <winrt/base.h>
#include <chrono>
#include <HexEngine/SDL/SDLWindow.hpp>
#include <HexEngine/Renderer/DirectX/DebugLayer.hpp>
#include <HexEngine/Renderer/DirectX/Device.hpp>
#include <HexEngine/Renderer/DirectX/CommandQueue.hpp>
#include <HexEngine/Renderer/DirectX/SwapChainManager.hpp>
#include <HexEngine/Renderer/DirectX/CommandList.hpp>

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
    #if defined(_DEBUG)
        static DebugLayer CreateDebugLayer();
    #endif
    
    static Device CreateDevice();
    static CommandQueue CreateCommandQueue(const Device& device, D3D12_COMMAND_LIST_TYPE type);
    static SwapChainManager CreateSwapChainManager(const SDLWindow& window, const Device& device, const CommandQueue& commandQueue, std::uint8_t bufferCount);
    static SwapChain CreateSwapChain(const SDLWindow& window, const CommandQueue& commandQueue, const std::uint32_t& width, const std::uint32_t& height, const std::uint32_t& bufferCount);
    static DescriptorHeap CreateDescriptorHeap(const Device& device, const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint32_t& numDescriptors);
    static std::vector<BackBuffer> CreateBackBuffers(const Device& device, const SwapChain& swapChain, const DescriptorHeap& descriptorHeap, std::uint64_t renderTargetDescriptorSize, std::uint8_t numBuffers);
    static CommandAllocator CreateCommandAllocator(const Device& device, D3D12_COMMAND_LIST_TYPE type);
    static CommandList CreateCommandList(const Device& device, const CommandAllocator& commandAllocator, D3D12_COMMAND_LIST_TYPE type);

    // Should Probably Be Moved Out Of Here
    static Fence CreateFence(const Device& device);
    static HANDLE CreateFenceEvent();
    static std::uint64_t Signal(const winrt::com_ptr<ID3D12CommandQueue>& commandQueue, const winrt::com_ptr<ID3D12Fence>& fence, std::uint64_t& fenceValue);
    static void WaitForFenceValue(const winrt::com_ptr<ID3D12Fence>& fence, const std::uint64_t& fenceValue, const HANDLE& fenceEvent, std::chrono::milliseconds duration = (std::chrono::milliseconds::max)());
    static void Flush(const winrt::com_ptr<ID3D12CommandQueue>& commandQueue, const winrt::com_ptr<ID3D12Fence>& fence, const HANDLE& fenceEvent, std::uint64_t& fenceValue);
};
