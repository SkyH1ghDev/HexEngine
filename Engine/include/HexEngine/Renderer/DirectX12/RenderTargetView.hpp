#pragma once
#include <directx/d3d12.h>
#include <wrl/client.h>

namespace MW = Microsoft::WRL;

class RenderTargetView
{
public:
    RenderTargetView() = default;
    ~RenderTargetView() = default;
    RenderTargetView(const RenderTargetView& other) = default;
    RenderTargetView& operator=(const RenderTargetView& other) = default;
    RenderTargetView(RenderTargetView&& other) noexcept = default;
    RenderTargetView& operator=(RenderTargetView&& other) noexcept = default;

    MW::ComPtr<ID3D12Resource> GetRTV();
    MW::ComPtr<ID3D12CommandAllocator> GetCommandAllocator();
private:

    MW::ComPtr<ID3D12Resource> m_rtv;
    MW::ComPtr<ID3D12CommandAllocator> m_commandAllocator;
};
