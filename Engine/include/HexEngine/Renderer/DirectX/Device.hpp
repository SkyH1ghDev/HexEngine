#pragma once
#include <DirectX/d3dx12.h>
#include <winrt/base.h>

class Device
{
public:
    Device() = default;
    explicit Device(const winrt::com_ptr<ID3D12Device9>& device);
    ~Device() = default;
    Device(const Device& other) = default;
    Device& operator=(const Device& other) = default;
    Device(Device&& other) noexcept = default;
    Device& operator=(Device&& other) noexcept = default;

    [[maybe_unused]] [[nodiscard]]
    winrt::com_ptr<ID3D12Device9> operator->() const;

    /**
     * 
     * @return WinRT ComPtr of ID3D12Device9
     */
    [[maybe_unused]] [[nodiscard]]
    winrt::com_ptr<ID3D12Device9> GetCOM() const;

    /**
     * 
     * @return Raw Ptr of ID3D12Device9
     * @note Prefer GetCOM() if possible
     */
    [[maybe_unused]] [[nodiscard]]
    ID3D12Device* GetRaw() const;
    
private:
    winrt::com_ptr<ID3D12Device9> m_device = nullptr;
    
};

inline winrt::com_ptr<ID3D12Device9> Device::operator->() const
{
    return m_device;
}


inline winrt::com_ptr<ID3D12Device9> Device::GetCOM() const
{
    return m_device;
}

inline ID3D12Device* Device::GetRaw() const
{
    return m_device.get();
}

