#pragma once
#include <HexEngine/Renderer/DirectX/Resource.hpp>
#include <HexEngine/Renderer/DirectX/CommandAllocator.hpp>

class BackBuffer
{
public:
    BackBuffer() = default;
    explicit BackBuffer(const Resource& renderTarget, const CommandAllocator& commandAllocator);
    ~BackBuffer() = default;
    BackBuffer(const BackBuffer& other) = default;
    BackBuffer& operator=(const BackBuffer& other) = default;
    BackBuffer(BackBuffer&& other) noexcept = default;
    BackBuffer& operator=(BackBuffer&& other) noexcept = default;

    [[maybe_unused]] [[nodiscard]]
    Resource GetRenderTarget() const;

    [[maybe_unused]] [[nodiscard]]
    CommandAllocator GetCommandAllocator() const;

    [[maybe_unused]] [[nodiscard]]
    std::uint64_t GetFenceValue() const;

    [[maybe_unused]]
    void SetFenceValue(std::uint64_t fenceValue);

private:
    Resource m_renderTarget;
    CommandAllocator m_commandAllocator;
    std::uint64_t m_fenceValue = 0;
    
};

inline Resource BackBuffer::GetRenderTarget() const
{
    return m_renderTarget;
}

inline CommandAllocator BackBuffer::GetCommandAllocator() const
{
    return m_commandAllocator;
}

inline std::uint64_t BackBuffer::GetFenceValue() const
{
    return m_fenceValue;
}

inline void BackBuffer::SetFenceValue(std::uint64_t fenceValue)
{
    m_fenceValue = fenceValue;
}