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

    [[maybe_unused]] [[nodiscard]]
    std::uint64_t GetFenceValue() const;

    [[maybe_unused]]
    void SetFenceValue(std::uint64_t fenceValue);

private:
    Resource m_renderTarget {};
    std::uint64_t m_fenceValue {0};
    
};

inline Resource& BackBuffer::GetRenderTarget() 
{
    return m_renderTarget;
}

inline std::uint64_t BackBuffer::GetFenceValue() const
{
    return m_fenceValue;
}

inline void BackBuffer::SetFenceValue(std::uint64_t fenceValue)
{
    m_fenceValue = fenceValue;
}