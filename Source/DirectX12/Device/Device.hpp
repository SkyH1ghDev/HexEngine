#pragma once

#include <d3d12.h>
#include <wrl.h>

namespace MS = Microsoft::WRL;

class Device
{
public:
    Device();

public:
    inline MS::ComPtr<ID3D12Device> GetDevice();

private:
    MS::ComPtr<ID3D12Device> _devicePtr;
};

inline MS::ComPtr<ID3D12Device> Device::GetDevice()
{
    return this->_devicePtr;
}

