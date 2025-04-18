#include "CommandQueue.hpp"

CommandQueue::CommandQueue(const winrt::com_ptr<ID3D12CommandQueue>& commandQueue)
{
    m_commandQueue = commandQueue;
}

void CommandQueue::ExecuteCommandLists()
{
    // Convert from vector of ComPtr to vector of raw pointers
    std::vector<ID3D12CommandList*> commandLists = { };
    commandLists.reserve(m_commandLists.size());
    
    for (const auto& commandList : m_commandLists)
    {
        commandLists.push_back(commandList.GetRaw());
    }
    
    m_commandQueue->ExecuteCommandLists(static_cast<std::uint32_t>(m_commandLists.size()), commandLists.data());

    m_commandLists.clear();
}

std::uint64_t CommandQueue::Signal(const Fence& fence, std::uint64_t fenceValue) const
{
    m_commandQueue->Signal(fence.GetRaw(), ++fenceValue);
    return fenceValue;
}

void CommandQueue::WaitForValue(const Fence& fence, std::uint64_t fenceValue) const
{
    m_commandQueue->Wait(fence.GetRaw(), fenceValue);
}
