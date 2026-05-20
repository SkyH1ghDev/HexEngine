#pragma once
#include <directx/d3dx12.h>
#include <winrt/base.h>

namespace HexEngine::Graphics::DirectX12
{
    class CommandAllocator
    {
    public:
        CommandAllocator() = default;
        explicit CommandAllocator(const winrt::com_ptr<ID3D12CommandAllocator>& commandAllocator);
        ~CommandAllocator() = default;
        CommandAllocator(const CommandAllocator& other) = default;
        CommandAllocator& operator=(const CommandAllocator& other) = default;
        CommandAllocator(CommandAllocator&& other) noexcept = default;
        CommandAllocator& operator=(CommandAllocator&& other) noexcept = default;

        winrt::com_ptr<ID3D12CommandAllocator> operator->();
    
        /**
         * 
         * @return WinRT ComPtr of ID3D12CommandAllocator
         */
        [[maybe_unused]] [[nodiscard]]
        winrt::com_ptr<ID3D12CommandAllocator>& GetCOM();

        /**
         * 
         * @return Raw Ptr of ID3D12CommandAllocator
         * @remarks Prefer GetCOM() if possible
         */
        [[maybe_unused]] [[nodiscard]]
        ID3D12CommandAllocator* GetRaw() const;

    
    private:
        winrt::com_ptr<ID3D12CommandAllocator> m_commandAllocator = nullptr;
    
    };

    inline winrt::com_ptr<ID3D12CommandAllocator> CommandAllocator::operator->()
    {
        return m_commandAllocator;
    }


    inline winrt::com_ptr<ID3D12CommandAllocator>& CommandAllocator::GetCOM()
    {
        return m_commandAllocator;
    }

    inline ID3D12CommandAllocator* CommandAllocator::GetRaw() const
    {
        return m_commandAllocator.get();
    }
}
