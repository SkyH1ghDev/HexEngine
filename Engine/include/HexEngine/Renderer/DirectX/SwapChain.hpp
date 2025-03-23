#pragma once
#include <winrt/base.h>
#include <dxgi1_6.h>

class SwapChain
{
public:
    SwapChain() = default;
    explicit SwapChain(const winrt::com_ptr<IDXGISwapChain4>& swapChain);
    ~SwapChain() = default;
    SwapChain(const SwapChain& other) = default;
    SwapChain& operator=(const SwapChain& other) = default;
    SwapChain(SwapChain&& other) noexcept = default;
    SwapChain& operator=(SwapChain&& other) noexcept = default;

    winrt::com_ptr<IDXGISwapChain4> operator->() const;
    
    [[maybe_unused]]
    [[nodiscard]]
    winrt::com_ptr<IDXGISwapChain4> GetCOM() const;
    
private:
    winrt::com_ptr<IDXGISwapChain4> m_swapChain = nullptr;
    
};

inline winrt::com_ptr<IDXGISwapChain4> SwapChain::operator->() const
{
    return m_swapChain;
}

inline winrt::com_ptr<IDXGISwapChain4> SwapChain::GetCOM() const
{
    return m_swapChain;
}
