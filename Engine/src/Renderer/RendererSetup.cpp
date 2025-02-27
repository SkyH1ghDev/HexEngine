#include "RendererSetup.hpp"

#include <HexEngine/Dev/DirectXHelper.hpp>
#include <dxgi1_6.h>

namespace DX = DirectX;

MW::ComPtr<ID3D12Debug> RendererSetup::CreateDebugLayer()
{
    // Enables Debug Layer
    #ifdef _DEBUG

        MW::ComPtr<ID3D12Debug> debugInterface;
        DX::ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));
        debugInterface->EnableDebugLayer();
        return debugInterface;
    
    #endif
}


MW::ComPtr<ID3D12Device> RendererSetup::CreateDevice()
{
    // Create IDXGIFactory
    
    MW::ComPtr<IDXGIFactory4> dxgiFactory;
    UINT createFactoryFlags = 0;
    #ifdef _DEBUG
        createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
    #endif
    
    DX::ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory)));

    // Create Adapter For GPU Interaction

    MW::ComPtr<IDXGIAdapter1> dxgiAdapter1;
    MW::ComPtr<IDXGIAdapter4> dxgiAdapter4;
    SIZE_T maxDedicatedVideoMemory = 0;

    for (UINT i = 0; dxgiFactory->EnumAdapters1(i, &dxgiAdapter1) != DXGI_ERROR_NOT_FOUND; ++i)
    {
        DXGI_ADAPTER_DESC1 desc;
        dxgiAdapter1->GetDesc1(&desc);

        if
        (
            (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0
            &&
            SUCCEEDED
            (
                D3D12CreateDevice
                (
                    dxgiAdapter1.Get(),
                    D3D_FEATURE_LEVEL_11_0,
                    __uuidof(ID3D12Device),
                    nullptr
                )
            )
            &&
            desc.DedicatedVideoMemory > maxDedicatedVideoMemory
        )
        {
            maxDedicatedVideoMemory = desc.DedicatedVideoMemory;
            DX::ThrowIfFailed(dxgiAdapter1.As(&dxgiAdapter4));
        }
    }
    
    // Create Device

    MW::ComPtr<ID3D12Device> device;
    DX::ThrowIfFailed(D3D12CreateDevice(dxgiAdapter1.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)));

    #ifdef _DEBUG
        
    MW::ComPtr<ID3D12InfoQueue> pInfoQueue;
    if (SUCCEEDED(device.As(&pInfoQueue)))
    {
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

        D3D12_MESSAGE_SEVERITY severities[] =
        {
            D3D12_MESSAGE_SEVERITY_INFO
        };

        D3D12_MESSAGE_ID denyIds[] =
        {
            D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
            D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,
            D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE
        };

        D3D12_INFO_QUEUE_FILTER filter = {};
        // filter.DenyList.NumCategories = _countof(Categories);
        // filter.DenyList.pCategoryList = Categories;
        filter.DenyList.NumSeverities = _countof(severities);
        filter.DenyList.pSeverityList = severities;
        filter.DenyList.NumIDs = _countof(denyIds);
        filter.DenyList.pIDList = denyIds;

        DX::ThrowIfFailed(pInfoQueue->PushStorageFilter(&filter));
    }
    
    #endif

    return device;
}
