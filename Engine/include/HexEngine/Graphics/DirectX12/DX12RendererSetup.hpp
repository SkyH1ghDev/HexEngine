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
        static CommandQueue CreateCommandQueue(const Device& pDevice, D3D12_COMMAND_LIST_TYPE pType);
        static SwapChainManager CreateSwapChainManager(const HexEngine::SDL::SDLWindow& pWindow, const Device& pDevice, const CommandQueue& pCommandQueue, uint8_t pBufferCount);
        static SwapChain CreateSwapChain(const HexEngine::SDL::SDLWindow& pWindow, const CommandQueue& pCommandQueue, const std::uint32_t& pBufferCount);
        static DescriptorHeap CreateDescriptorHeap(const Device& pDevice, const D3D12_DESCRIPTOR_HEAP_TYPE& pType, const std::uint32_t& pNumDescriptors);
        static std::vector<BackBuffer> CreateBackBuffers(const Device& pDevice, const SwapChain& pSwapChain, const DescriptorHeap& pDescriptorHeap, uint64_t
                                                         pRenderTargetDescriptorSize, uint8_t numBuffers);
        static CommandAllocator CreateCommandAllocator(const Device& device, D3D12_COMMAND_LIST_TYPE type);
        static CommandList CreateCommandList(const Device& device, const CommandAllocator& commandAllocator, D3D12_COMMAND_LIST_TYPE type);

        // Should Probably Be Moved Out Of Here
        static Fence CreateFence(const Device& device);
        static HANDLE CreateFenceEvent();
        static void WaitForFenceValue(const winrt::com_ptr<ID3D12Fence>& fence, const std::uint64_t& fenceValue, const HANDLE& fenceEvent, std::chrono::milliseconds duration = (std::chrono::milliseconds::max)());
    };
}
