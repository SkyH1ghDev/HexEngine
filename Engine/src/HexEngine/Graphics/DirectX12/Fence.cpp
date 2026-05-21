#include "Fence.hpp"
#include <HexEngine/Utils/DirectXUtils.hpp>

// False-Positive that says this is unused when it clearly is being used below
namespace DXUtils = DirectXUtils;
using namespace HexEngine::Graphics::DirectX12;

Fence::Fence(const winrt::com_ptr<ID3D12Fence1>& fence, const HANDLE& fenceEvent)
{
    this->mFence = fence;
    this->mFenceEventHandle = fenceEvent;
}

std::uint64_t Fence::Signal(std::uint64_t fenceValue) const
{
    DXUtils::ThrowIfFailed(mFence->Signal(++fenceValue));
    return fenceValue;
}

void Fence::WaitForValue(std::uint64_t fenceValue, const std::chrono::milliseconds duration) const
{
    if (mFence->GetCompletedValue() < fenceValue)
    {
        DXUtils::ThrowIfFailed(mFence->SetEventOnCompletion(fenceValue, mFenceEventHandle));
        ::WaitForSingleObject(mFenceEventHandle, static_cast<DWORD>(duration.count()));
    }
}

void Fence::Flush(std::uint64_t fenceValue) const
{
    fenceValue = Signal(fenceValue);
    WaitForValue(fenceValue);
}