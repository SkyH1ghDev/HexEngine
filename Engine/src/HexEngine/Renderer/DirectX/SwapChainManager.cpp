#include "SwapChainManager.hpp"

#include <HexEngine/Renderer/DirectX/DirectXUtils.hpp>

namespace DXUtils = DirectXUtils;

SwapChainManager::SwapChainManager(const SwapChain& swapChain, const DescriptorHeap& descriptorHeap, const std::vector<BackBuffer>& backBuffers, std::uint32_t renderTargetDescriptorSize)
{
    m_swapChain = swapChain;
    m_descriptorHeap = descriptorHeap;
    m_backBuffers = backBuffers;
    m_renderTargetDescriptorSize = renderTargetDescriptorSize;
}

void SwapChainManager::PresentFrame(bool doVSync) const
{
    const std::uint32_t syncInterval = doVSync ? 1 : 0;
    const std::uint32_t presentFlags = DXUtils::CheckTearingSupport() && !doVSync ? DXGI_PRESENT_ALLOW_TEARING : 0;
        
    DXUtils::ThrowIfFailed(m_swapChain->Present(syncInterval, presentFlags));
}
