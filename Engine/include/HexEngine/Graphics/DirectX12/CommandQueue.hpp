#pragma once
#include <directx/d3dx12.h>
#include <winrt/base.h>
#include <HexEngine/Graphics/DirectX12/Fence.hpp>
#include <HexEngine/Graphics/DirectX12/CommandList.hpp>

namespace HexEngine::Graphics::DirectX12
{
    class CommandQueue
    {
    public:
        CommandQueue() = default;
        explicit CommandQueue(const winrt::com_ptr<ID3D12CommandQueue>& commandQueue);
        ~CommandQueue() = default;
        CommandQueue(const CommandQueue& other) = default;
        CommandQueue& operator=(const CommandQueue& other) = default;
        CommandQueue(CommandQueue&& other) noexcept = default;
        CommandQueue& operator=(CommandQueue&& other) noexcept = default;
        
        winrt::com_ptr<ID3D12CommandQueue> operator->() const;

        [[maybe_unused]] [[nodiscard]]
        winrt::com_ptr<ID3D12CommandQueue> GetCOM() const;
    
        void ExecuteCommandLists(std::vector<CommandList>& pCommandLists) const;

        // GPU Sync
        [[nodiscard]]
        std::uint64_t GPUSignal(const Fence& fence, std::uint64_t fenceValue) const;
        void GPUWaitForValue(const Fence& fence, std::uint64_t fenceValue) const;

    private:
        winrt::com_ptr<ID3D12CommandQueue> mCommandQueue = nullptr;
    };

    inline winrt::com_ptr<ID3D12CommandQueue> CommandQueue::GetCOM() const
    {
        return mCommandQueue;
    }
}
