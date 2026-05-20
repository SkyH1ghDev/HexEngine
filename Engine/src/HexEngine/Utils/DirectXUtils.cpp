#include "DirectXUtils.hpp"
#include <winrt/base.h>
#include <dxgi1_6.h>
#include <format>

using namespace winrt;

DirectXUtils::DirectXException::DirectXException(HRESULT hr) :
    m_result(hr),
    m_message(std::format("Failure with HRESULT of {:08X}", static_cast<std::uint32_t>(m_result)))
{
    
}

const char* DirectXUtils::DirectXException::what() const noexcept
{
    return m_message.c_str();
}

bool DirectXUtils::CheckTearingSupport()
{
    bool allowTearing = false;

    com_ptr<IDXGIFactory4> factory4;
    if (!SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory4))))
    {
        return false;
    }
    
    com_ptr<IDXGIFactory5> factory5 = nullptr;
    try
    {
        factory5 = factory4.as<IDXGIFactory5>();
    }
    catch ([[maybe_unused]] winrt::hresult_error const& ex)
    {
		//winrt::hresult hr = ex.code();
		//winrt::hstring message = ex.message();
		return false;
	}
    
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
        allowTearing = false;
    }

    return allowTearing;
}

bool DirectXUtils::CheckMeshShaderSupport(com_ptr<ID3D12Device9> &device)
{
    D3D12_FEATURE_DATA_D3D12_OPTIONS7 features = {};

    if (FAILED(device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS7, &features, sizeof(features))))
        return false;

    return features.MeshShaderTier != D3D12_MESH_SHADER_TIER_NOT_SUPPORTED;
}
