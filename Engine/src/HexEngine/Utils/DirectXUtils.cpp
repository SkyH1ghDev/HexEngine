#include "DirectXUtils.hpp"
#include <wrl/client.h>
#include <dxgi1_6.h>
#include <format>

namespace MW = Microsoft::WRL;

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

    MW::ComPtr<IDXGIFactory4> factory4;
    if (!SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory4))))
    {
        return false;
    }
    
    MW::ComPtr<IDXGIFactory5> factory5;
    if (!SUCCEEDED(factory4.As(&factory5)))
    {
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
