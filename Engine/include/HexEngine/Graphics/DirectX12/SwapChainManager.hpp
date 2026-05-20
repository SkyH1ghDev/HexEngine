#pragma once
#include "DescriptorHeap.hpp"
#include "BackBuffer.hpp"
#include "SwapChain.hpp"

namespace HexEngine::Graphics::DirectX12
{
    class SwapChainManager
    {
    public:
        SwapChainManager() = default;
        explicit SwapChainManager(const SwapChain& swapChain, const DescriptorHeap& descriptorHeap, const std::vector<BackBuffer>& backBuffers, std::uint32_t renderTargetDescriptorSize);
        ~SwapChainManager() = default;
        SwapChainManager(const SwapChainManager& other) = default;
        SwapChainManager& operator=(const SwapChainManager& other) = default;
        SwapChainManager(SwapChainManager&& other) noexcept = default;
        SwapChainManager& operator=(SwapChainManager&& other) noexcept = default;

        void PresentFrame(bool doVSync) const;
        void UpdateBackBufferIndex();
        
        [[maybe_unused]] [[nodiscard]]
        DescriptorHeap& GetDescriptorHeap();

        [[maybe_unused]] [[nodiscard]]
        BackBuffer& GetBackBufferAt(std::uint8_t index);
        
        [[maybe_unused]] [[nodiscard]]
        BackBuffer& GetCurrentBackBuffer();
        
        [[maybe_unused]] [[nodiscard]]
        std::vector<BackBuffer>& GetBackBuffers();

        [[maybe_unused]] [[nodiscard]]
        std::uint32_t GetCurrentBackBufferIndex() const;

        [[maybe_unused]] [[nodiscard]]
        std::uint32_t GetRenderTargetDescriptorSize() const;
        
    private:
        SwapChain m_swapChain {};
        DescriptorHeap m_descriptorHeap {};
        std::uint32_t m_renderTargetDescriptorSize {0};
        std::vector<BackBuffer> m_backBuffers {};
        std::uint32_t m_currentBackBufferIndex {0};
        
    };

    inline void SwapChainManager::UpdateBackBufferIndex()
    {
        m_currentBackBufferIndex = m_swapChain->GetCurrentBackBufferIndex();
    }

    inline DescriptorHeap& SwapChainManager::GetDescriptorHeap()
    {
        return m_descriptorHeap;
    }

    inline BackBuffer& SwapChainManager::GetBackBufferAt(std::uint8_t index)
    {
        return m_backBuffers.at(index);
    }

    inline BackBuffer& SwapChainManager::GetCurrentBackBuffer()
    {
        return m_backBuffers.at(m_currentBackBufferIndex);
    }

    inline std::vector<BackBuffer>& SwapChainManager::GetBackBuffers()
    {
        return m_backBuffers;
    }

    inline std::uint32_t SwapChainManager::GetCurrentBackBufferIndex() const
    {
        return m_currentBackBufferIndex;
    }

    inline std::uint32_t SwapChainManager::GetRenderTargetDescriptorSize() const
    {
        return m_renderTargetDescriptorSize;
    }   
}
