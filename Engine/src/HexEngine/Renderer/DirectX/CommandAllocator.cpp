#include "CommandAllocator.hpp"

CommandAllocator::CommandAllocator(const winrt::com_ptr<ID3D12CommandAllocator>& commandAllocator)
{
    m_commandAllocator = commandAllocator;
}
