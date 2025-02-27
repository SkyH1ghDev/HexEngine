#pragma once

#include <wrl/client.h>
#include <directx/d3dx12.h>

namespace MW = Microsoft::WRL;

class RendererSetup
{
public:
    RendererSetup() = default;
    ~RendererSetup() = default;
    RendererSetup(const RendererSetup& other) = default;
    RendererSetup& operator=(const RendererSetup& other) = default;
    RendererSetup(RendererSetup&& other) noexcept = default;
    RendererSetup& operator=(RendererSetup&& other) noexcept = default;

    // DEBUG
    static MW::ComPtr<ID3D12Debug> CreateDebugLayer();
    
    static MW::ComPtr<ID3D12Device> CreateDevice();
};
