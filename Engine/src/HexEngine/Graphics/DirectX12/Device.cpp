#include "Device.hpp"

using namespace HexEngine::Graphics::DirectX12;

Device::Device(const winrt::com_ptr<ID3D12Device9>& device)
{
    m_device = device;
}
