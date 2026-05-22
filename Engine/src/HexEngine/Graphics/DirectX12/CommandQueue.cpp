#include "CommandQueue.hpp"

using namespace HexEngine::Graphics::DirectX12;

CommandQueue::CommandQueue(const winrt::com_ptr<ID3D12CommandQueue>& commandQueue)
{
    mCommandQueue = commandQueue;
}

winrt::com_ptr<ID3D12CommandQueue> CommandQueue::operator->() const
{
    return mCommandQueue;
}

void CommandQueue::ExecuteCommandLists([[maybe_unused]] std::vector<CommandList>& pCommandList) const
{
    // TODO: FIX THIS :)
    
    //mCommandQueue->ExecuteCommandLists(static_cast<std::uint32_t>(pCommandList.size()), pCommandList.data());
    //pCommandList.clear();
}

std::uint64_t CommandQueue::GPUSignal(const Fence& fence, std::uint64_t fenceValue) const
{
    mCommandQueue->Signal(fence.GetRaw(), ++fenceValue);
    return fenceValue;
}

void CommandQueue::GPUWaitForValue(const Fence& fence, std::uint64_t fenceValue) const
{
    mCommandQueue->Wait(fence.GetRaw(), fenceValue);
}
