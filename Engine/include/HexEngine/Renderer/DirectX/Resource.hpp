#pragma once
#include <directx/d3dx12.h>
#include <winrt/base.h>

class Resource
{
public:
    Resource() = default;
    Resource(const winrt::com_ptr<ID3D12Resource2>& resource);
    ~Resource() = default;
    Resource(const Resource& other) = default;
    Resource& operator=(const Resource& other) = default;
    Resource(Resource&& other) noexcept = default;
    Resource& operator=(Resource&& other) noexcept = default;

    /**
     * 
     * @return WinRT ComPtr of ID3D12Resource
     */
    [[maybe_unused]] [[nodiscard]]
    winrt::com_ptr<ID3D12Resource2> GetCOM() const;

    /**
     * 
     * @return Raw Ptr of ID3D12Resource
     * @note Prefer GetCOM() if possible
     */
    [[maybe_unused]] [[nodiscard]]
    ID3D12Resource2* GetRaw() const;
    
private:
    winrt::com_ptr<ID3D12Resource2> m_resource = nullptr;
    
};

inline winrt::com_ptr<ID3D12Resource2> Resource::GetCOM() const
{
    return m_resource;
}

inline ID3D12Resource2* Resource::GetRaw() const
{
    return m_resource.get();
}