#include "DescriptorHeap.hpp"

using namespace HexEngine::Graphics::DirectX12;

DescriptorHeap::DescriptorHeap(const winrt::com_ptr<ID3D12DescriptorHeap>& descriptorHeap)
{
    m_descriptorHeap = descriptorHeap;
}
