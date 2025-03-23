#include "BackBuffer.hpp"

BackBuffer::BackBuffer(const Resource& renderTarget, const CommandAllocator& commandAllocator)
{
    m_renderTarget = renderTarget;
    m_commandAllocator = commandAllocator;
}
