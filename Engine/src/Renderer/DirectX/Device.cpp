#include "Device.hpp"

Device::Device(const winrt::com_ptr<ID3D12Device9>& device)
{
    m_device = device;
}
