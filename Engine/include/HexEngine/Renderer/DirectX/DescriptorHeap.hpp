#pragma once
#include <directx/d3dx12.h>
#include <winrt/base.h>

class DescriptorHeap
{
public:
    DescriptorHeap() = default;
    explicit DescriptorHeap(const winrt::com_ptr<ID3D12DescriptorHeap>& descriptorHeap);
    ~DescriptorHeap() = default;
    DescriptorHeap(const DescriptorHeap& other) = default;
    DescriptorHeap& operator=(const DescriptorHeap& other) = default;
    DescriptorHeap(DescriptorHeap&& other) noexcept = default;
    DescriptorHeap& operator=(DescriptorHeap&& other) noexcept = default;

    winrt::com_ptr<ID3D12DescriptorHeap> operator->() const;
    
    /**
     * 
     * @return WinRT ComPtr of ID3D12DescriptorHeap
     */
    [[maybe_unused]] [[nodiscard]]
    winrt::com_ptr<ID3D12DescriptorHeap> GetCOM() const;

    /**
     * 
     * @return Raw Ptr of ID3D12DescriptorHeap
     * @note Prefer GetCOM() if possible
     */
    [[maybe_unused]] [[nodiscard]]
    ID3D12DescriptorHeap* GetRaw() const;

private:
    winrt::com_ptr<ID3D12DescriptorHeap> m_descriptorHeap = nullptr;
    
};

inline winrt::com_ptr<ID3D12DescriptorHeap> DescriptorHeap::operator->() const
{
    return m_descriptorHeap;
}

inline winrt::com_ptr<ID3D12DescriptorHeap> DescriptorHeap::GetCOM() const
{
    return m_descriptorHeap;
}

inline ID3D12DescriptorHeap* DescriptorHeap::GetRaw() const
{
    return m_descriptorHeap.get();
}
