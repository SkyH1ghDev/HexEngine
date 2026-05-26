#include <PCH/EnginePCH.hpp>
#include "DebugLayer.hpp"

using namespace HexEngine::Graphics::DirectX12;

DebugLayer::DebugLayer(const winrt::com_ptr<ID3D12Debug6>& debugLayer)
{
    m_debugLayer = debugLayer;
}
