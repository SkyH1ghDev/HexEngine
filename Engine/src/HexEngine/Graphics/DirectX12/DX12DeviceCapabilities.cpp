#include "DX12DeviceCapabilities.hpp"

bool DX12DeviceCapabilities::CheckTearingSupport()
{

    winrt::com_ptr<IDXGIFactory4> factory4;
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory4))))
    {
        return false;
    }
    
    winrt::com_ptr<IDXGIFactory5> factory5 {nullptr};
    try
    {
        factory5 = factory4.as<IDXGIFactory5>();
    }
    catch (winrt::hresult_error const& ex)
    {
        std::string message = StringUtils::HResultToString(ex.code());
        std::print("ErrorCode: {0}, Message: {1}\n", static_cast<std::int32_t>(ex.code()), message);
        return false;
    }
    
    bool allowTearing = false;
    
    if
    (
        FAILED 
        (
            factory5->CheckFeatureSupport
            (
                DXGI_FEATURE_PRESENT_ALLOW_TEARING, 
                &allowTearing,
                sizeof(allowTearing)
            )
        )
    )
    {
        return false;
    }

    return true;
}

bool DX12DeviceCapabilities::CheckMeshShaderSupport(const Device& pDevice)
{
    D3D12_FEATURE_DATA_D3D12_OPTIONS7 features = {};

    if (FAILED(pDevice->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS7, &features, sizeof(features))))
        return false;

    return features.MeshShaderTier != D3D12_MESH_SHADER_TIER_NOT_SUPPORTED;
}

bool DX12DeviceCapabilities::CheckDirectXMathSupport()
{
    return DirectX::XMVerifyCPUSupport();
}
