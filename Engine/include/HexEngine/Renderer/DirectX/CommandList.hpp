#pragma once
#include <winrt/base.h>
#include <directx/d3dx12.h>

class CommandList
{
public:
    CommandList() = default;
    explicit CommandList(const winrt::com_ptr<ID3D12GraphicsCommandList6>& commandList);
    ~CommandList() = default;
    CommandList(const CommandList& other) = default;
    CommandList& operator=(const CommandList& other) = default;
    CommandList(CommandList&& other) noexcept = default;
    CommandList& operator=(CommandList&& other) noexcept = default;

    winrt::com_ptr<ID3D12GraphicsCommandList6> operator->() const;
    
    /**
     * 
     * @return WinRT ComPtr of ID3D12GraphicsCommandList6
     */
    [[maybe_unused]] [[nodiscard]]
    winrt::com_ptr<ID3D12GraphicsCommandList6> GetCOM() const;

    /**
     * 
     * @return Raw Ptr of ID3D12GraphicsCommandList6
     * @note Prefer GetCOM() if possible
     */
    [[maybe_unused]] [[nodiscard]]
    ID3D12GraphicsCommandList6* GetRaw() const;
    
private:
    winrt::com_ptr<ID3D12GraphicsCommandList6> m_commandList;
    
};

inline winrt::com_ptr<ID3D12GraphicsCommandList6> CommandList::operator->() const
{
    return m_commandList;
}


inline winrt::com_ptr<ID3D12GraphicsCommandList6> CommandList::GetCOM() const
{
    return m_commandList;
}

inline ID3D12GraphicsCommandList6* CommandList::GetRaw() const
{
    return m_commandList.get();
}
