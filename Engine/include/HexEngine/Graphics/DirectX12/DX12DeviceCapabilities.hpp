#pragma once

namespace HexEngine::Graphics::DirectX12
{
    class DX12DeviceCapabilities
    {
    public:
        DX12DeviceCapabilities() = default;
        ~DX12DeviceCapabilities() = default;
        DX12DeviceCapabilities(const DX12DeviceCapabilities&) = delete;
        DX12DeviceCapabilities& operator=(const DX12DeviceCapabilities&) = delete;
        DX12DeviceCapabilities(DX12DeviceCapabilities&&) = delete;
        DX12DeviceCapabilities& operator=(DX12DeviceCapabilities&&) = delete;
    
        static bool CheckTearingSupport();
        static bool CheckMeshShaderSupport(const Device& pDevice);
        static bool CheckDirectXMathSupport();
    
    private:
    };
}
