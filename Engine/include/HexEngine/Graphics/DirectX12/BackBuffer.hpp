#pragma once
#include <HexEngine/Graphics/DirectX12/Resource.hpp>
#include <HexEngine/Graphics/DirectX12/CommandAllocator.hpp>

using namespace HexEngine::Graphics::DirectX12;

class BackBuffer
{
public:
    BackBuffer() = default;
    explicit BackBuffer(const Resource& renderTarget);
    ~BackBuffer() = default;
    BackBuffer(const BackBuffer& other) = default;
    BackBuffer& operator=(const BackBuffer& other) = default;
    BackBuffer(BackBuffer&& other) noexcept = default;
    BackBuffer& operator=(BackBuffer&& other) noexcept = default;

    [[maybe_unused]] [[nodiscard]]
    Resource& GetRenderTarget();

private:
    Resource m_renderTarget {};
    
};

inline Resource& BackBuffer::GetRenderTarget() 
{
    return m_renderTarget;
}