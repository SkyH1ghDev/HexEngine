#pragma once
#include <directx/d3dx12.h>
#include <winrt/base.h>
#include <HexEngine/Renderer/DirectX/Fence.hpp>
#include <HexEngine/Renderer/DirectX/CommandList.hpp>

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

    [[maybe_unused]] [[nodiscard]]
    winrt::com_ptr<ID3D12CommandQueue> GetCOM() const;
    
    void AppendCommandList(const CommandList& commandList);
    void ExecuteCommandLists();

    // GPU Sync
    std::uint64_t Signal(const Fence& fence, std::uint64_t fenceValue) const;
    void WaitForValue(const Fence& fence, std::uint64_t fenceValue) const;

private:
    winrt::com_ptr<ID3D12CommandQueue> m_commandQueue = nullptr;
    std::vector<CommandList> m_commandLists = { };
    
};

inline winrt::com_ptr<ID3D12CommandQueue> CommandQueue::GetCOM() const
{
    return m_commandQueue;
}

inline void CommandQueue::AppendCommandList(const CommandList& commandList)
{
    m_commandLists.push_back(commandList);
}
