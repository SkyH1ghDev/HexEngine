#include "DescriptorHeap.hpp"

DescriptorHeap::DescriptorHeap(const winrt::com_ptr<ID3D12DescriptorHeap>& descriptorHeap)
{
    m_descriptorHeap = descriptorHeap;
}
