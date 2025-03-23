#include "SwapChain.hpp"

SwapChain::SwapChain(const winrt::com_ptr<IDXGISwapChain4>& swapChain)
{
    m_swapChain = swapChain;
}
