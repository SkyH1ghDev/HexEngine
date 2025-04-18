#include "Fence.hpp"
#include <HexEngine/Renderer/DirectX/DirectXUtils.hpp>
#include <HexEngine/Renderer/DirectX/CommandQueue.hpp>

// False-Positive that says this is unused when it clearly is being used below
namespace DXUtils = DirectXUtils;           // NOLINT(misc-unused-alias-decls)

Fence::Fence(const winrt::com_ptr<ID3D12Fence1>& fence, const HANDLE& fenceEvent)
{
    this->m_fence = fence;
    this->m_fenceEvent = fenceEvent;
}

std::uint64_t Fence::Signal(std::uint64_t fenceValue) const
{
    DXUtils::ThrowIfFailed(m_fence->Signal(++fenceValue));
    return fenceValue;
}

void Fence::WaitForValue(std::uint64_t fenceValue, const std::chrono::milliseconds duration) const
{
    if (m_fence->GetCompletedValue() < fenceValue)
    {
        DXUtils::ThrowIfFailed(m_fence->SetEventOnCompletion(fenceValue, m_fenceEvent));
        ::WaitForSingleObject(m_fenceEvent, static_cast<DWORD>(duration.count()));
    }
}

void Fence::Flush(std::uint64_t fenceValue) const
{
    fenceValue = Signal(fenceValue);
    WaitForValue(fenceValue);
}