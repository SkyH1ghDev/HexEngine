#include <PCH/EnginePCH.hpp>
#include "BackBuffer.hpp"

BackBuffer::BackBuffer(const Resource& renderTarget)
{
    m_renderTarget = renderTarget;
}