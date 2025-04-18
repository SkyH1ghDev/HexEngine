#include "DebugLayer.hpp"

DebugLayer::DebugLayer(const winrt::com_ptr<ID3D12Debug6>& debugLayer)
{
    m_debugLayer = debugLayer;
}
