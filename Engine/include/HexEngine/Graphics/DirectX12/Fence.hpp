#pragma once
#include <cstdint>
#include <directx/d3dx12.h>
#include <winrt/base.h>
#include <chrono>

namespace HexEngine::Graphics::DirectX12
{
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
        std::uint64_t GetFenceValue() const;
        void SetFenceValue(std::uint64_t pFenceValue);

        std::uint64_t Signal(std::uint64_t pFenceValue) const;
        void WaitForValue(std::uint64_t fenceValue, std::chrono::milliseconds duration = (std::chrono::milliseconds::max)()) const;
        void Flush(std::uint64_t fenceValue) const;

    private:
        winrt::com_ptr<ID3D12Fence1> mFence = nullptr;
        HANDLE mFenceEventHandle = nullptr;
        std::uint64_t mFenceTargetValue = 0;
    };

    inline winrt::com_ptr<ID3D12Fence1> Fence::GetCOM() const
    {
        return mFence;
    }

    inline ID3D12Fence1* Fence::GetRaw() const
    {
        return mFence.get();
    }
    
    inline std::uint64_t Fence::GetFenceValue() const
    {
        return mFenceTargetValue;
    }
    
    inline void Fence::SetFenceValue(std::uint64_t pFenceValue)
    {
        mFenceTargetValue = pFenceValue;
    }
}
