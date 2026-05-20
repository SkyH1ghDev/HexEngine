#pragma once

#include <directx/d3dx12.h>
#include <winrt/base.h>
#include <chrono>
#include <HexEngine/SDL/SDLWindow.hpp>
#include <HexEngine/Graphics/DirectX12/Fence.hpp>
#include <HexEngine/Graphics/DirectX12/DebugLayer.hpp>
#include <HexEngine/Graphics/DirectX12/Device.hpp>
#include <HexEngine/Graphics/DirectX12/CommandQueue.hpp>
#include <HexEngine/Graphics/DirectX12/SwapChainManager.hpp>
#include <HexEngine/Graphics/DirectX12/CommandList.hpp>

namespace HexEngine::Graphics::DirectX12
{
    class DX12RendererSetup
    {
    public:
        DX12RendererSetup() = default;
        ~DX12RendererSetup() = default;
        DX12RendererSetup(const DX12RendererSetup& other) = default;
        DX12RendererSetup& operator=(const DX12RendererSetup& other) = default;
        DX12RendererSetup(DX12RendererSetup&& other) noexcept = default;
        DX12RendererSetup& operator=(DX12RendererSetup&& other) noexcept = default;

        // DEBUG
        #if defined(_DEBUG)
            static DebugLayer CreateDebugLayer();
        #endif
        
        static Device CreateDevice();
        static CommandQueue CreateCommandQueue(const Device& device, D3D12_COMMAND_LIST_TYPE type);
        static SwapChainManager CreateSwapChainManager(const HexEngine::SDL::SDLWindow& window, const Device& device, const CommandQueue& commandQueue, std::uint8_t bufferCount);
        static SwapChain CreateSwapChain(const HexEngine::SDL::SDLWindow& window, const CommandQueue& commandQueue, const std::uint32_t& width, const std::uint32_t& height, const std::uint32_t& bufferCount);
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
}
