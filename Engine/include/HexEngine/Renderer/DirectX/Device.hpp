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
    
    [[maybe_unused]] [[nodiscard]]
    winrt::com_ptr<ID3D12Device9> GetCOM() const;
    
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
