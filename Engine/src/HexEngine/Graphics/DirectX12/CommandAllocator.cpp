#include "CommandAllocator.hpp"

using namespace HexEngine::Graphics::DirectX12;

CommandAllocator::CommandAllocator(const winrt::com_ptr<ID3D12CommandAllocator>& commandAllocator)
{
    m_commandAllocator = commandAllocator;
}
