#include "SwapChain.hpp"

using namespace HexEngine::Graphics::DirectX12;

SwapChain::SwapChain(const winrt::com_ptr<IDXGISwapChain4>& swapChain)
{
    m_swapChain = swapChain;
}
