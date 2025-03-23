#pragma once
#include <cstdint>
#include <directx/d3dx12.h>
#include <winrt/base.h>
#include <chrono>


class Fence
{
public:
    Fence() = default;
    explicit Fence(const winrt::com_ptr<ID3D12Fence1>& fence, const HANDLE& fenceEvent);
    ~Fence() = default;
    Fence(const Fence& other) = default;
    Fence& operator=(const Fence& other) = default;
    Fence(Fence&& other) noexcept = default;
    Fence& operator=(Fence&& other) noexcept = default;

    /**
     * 
     * @return WinRT ComPtr of ID3D12Fence1
     */
    [[maybe_unused]] [[nodiscard]]
    winrt::com_ptr<ID3D12Fence1> GetCOM() const;

    /**
     * 
     * @return Raw Ptr of ID3D12Fence1
     * @note Prefer GetCOM() if possible
     */
    [[maybe_unused]] [[nodiscard]]
    ID3D12Fence1* GetRaw() const;

    std::uint64_t Signal(std::uint64_t fenceValue) const;
    void WaitForValue(std::uint64_t fenceValue, std::chrono::milliseconds duration = (std::chrono::milliseconds::max)()) const;
    std::uint64_t Flush(std::uint64_t fenceValue);

private:
    winrt::com_ptr<ID3D12Fence1> m_fence = nullptr;
    HANDLE m_fenceEvent = nullptr;
};

inline winrt::com_ptr<ID3D12Fence1> Fence::GetCOM() const
{
    return m_fence; 
}

inline ID3D12Fence1* Fence::GetRaw() const
{
    return m_fence.get();
}