#include <PCH/EnginePCH.hpp>
#include "Resource.hpp"

using namespace HexEngine::Graphics::DirectX12;

Resource::Resource(const winrt::com_ptr<ID3D12Resource2>& resource)
{
    m_resource = resource;
}
