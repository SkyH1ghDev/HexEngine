#pragma once
#include <directx/d3dx12.h>
#include <winrt/base.h>

class DebugLayer
{
public:
    DebugLayer() = default;
    explicit DebugLayer(const winrt::com_ptr<ID3D12Debug6>& debugLayer);
    ~DebugLayer() = default;
    DebugLayer(const DebugLayer& other) = default;
    DebugLayer& operator=(const DebugLayer& other) = default;
    DebugLayer(DebugLayer&& other) noexcept = default;
    DebugLayer& operator=(DebugLayer&& other) noexcept = default;

    [[maybe_unused]]
    [[nodiscard]]
    winrt::com_ptr<ID3D12Debug6> GetCOM() const;
    
private:
    winrt::com_ptr<ID3D12Debug6> m_debugLayer = nullptr;
};

inline winrt::com_ptr<ID3D12Debug6> DebugLayer::GetCOM() const
{
    return m_debugLayer;
}