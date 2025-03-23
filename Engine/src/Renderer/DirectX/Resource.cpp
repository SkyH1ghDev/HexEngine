#include "Resource.hpp"

Resource::Resource(const winrt::com_ptr<ID3D12Resource2>& resource)
{
    m_resource = resource;
}
