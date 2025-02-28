#include "RendererSetup.hpp"

#include <HexEngine/Dev/DirectXHelper.hpp>
#include <dxgi1_6.h>
#include <SDL3/SDL.h>
#include <cassert>

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

MW::ComPtr<ID3D12Device2> RendererSetup::CreateDevice()
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

    MW::ComPtr<ID3D12Device2> device;
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

MW::ComPtr<ID3D12CommandQueue> RendererSetup::CreateCommandQueue(const MW::ComPtr<ID3D12Device2>& device, const D3D12_COMMAND_LIST_TYPE& type)
{
    MW::ComPtr<ID3D12CommandQueue> commandQueue;

    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type =        type;
    queueDesc.Priority =    D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    queueDesc.Flags =       D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.NodeMask =    0;

    DX::ThrowIfFailed(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue)));

    return commandQueue;
}   

MW::ComPtr<IDXGISwapChain4> RendererSetup::CreateSwapChain(const std::shared_ptr<SDL_Window>& window, const MW::ComPtr<ID3D12CommandQueue>& commandQueue, const std::uint32_t& width, const std::uint32_t& height, const std::uint32_t& bufferCount)
{
    MW::ComPtr<IDXGISwapChain4> swapChain;
    MW::ComPtr<IDXGIFactory4> factory;

    UINT createFactoryFlags = 0;

    #ifdef _DEBUG
    createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
    #endif

    DX::ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&factory)));
    

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.Width =           width;
    swapChainDesc.Height =          height;
    swapChainDesc.Format =          DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.Stereo =          false;
    swapChainDesc.SampleDesc =  {.Count = 1, .Quality = 0};
    swapChainDesc.BufferCount =     bufferCount;
    swapChainDesc.BufferUsage =     DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount =     bufferCount;
    swapChainDesc.Scaling =         DXGI_SCALING_STRETCH;
    swapChainDesc.SwapEffect =      DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.AlphaMode =       DXGI_ALPHA_MODE_UNSPECIFIED;
    swapChainDesc.Flags =           CheckTearingSupport() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

    const HWND hWnd = static_cast<HWND>(SDL_GetPointerProperty(SDL_GetWindowProperties(window.get()), SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr));
    
    MW::ComPtr<IDXGISwapChain1> swapChain1;
    DX::ThrowIfFailed
    (
        factory->CreateSwapChainForHwnd
        (
            commandQueue.Get(),
            hWnd,
            &swapChainDesc,
            nullptr,
            nullptr,
            &swapChain1
        )
    );

    DX::ThrowIfFailed(factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));
    DX::ThrowIfFailed(swapChain1.As(&swapChain));

    return swapChain;
}

MW::ComPtr<ID3D12DescriptorHeap> RendererSetup::CreateDescriptorHeap(const MW::ComPtr<ID3D12Device2>& device, const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint32_t& numDescriptors)
{
    MW::ComPtr<ID3D12DescriptorHeap> descriptorHeap;

    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.NumDescriptors = numDescriptors;
    desc.Type = type;

    DX::ThrowIfFailed(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptorHeap)));

    return descriptorHeap;
}

std::vector<MW::ComPtr<ID3D12Resource>> RendererSetup::CreateBackBuffers(const MW::ComPtr<ID3D12Device2>& device, const MW::ComPtr<IDXGISwapChain4>& swapChain, const MW::ComPtr<ID3D12DescriptorHeap>& descriptorHeap, const std::uint8_t& numBuffers)
{
    UINT rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(descriptorHeap->GetCPUDescriptorHandleForHeapStart());

    std::vector<MW::ComPtr<ID3D12Resource>> backBuffers;
    
    for (int i = 0; i < numBuffers; ++i)
    {
        MW::ComPtr<ID3D12Resource> backBuffer;
        DX::ThrowIfFailed(swapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)));

        device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtvHandle);

        backBuffers.push_back(backBuffer);

        rtvHandle.Offset(static_cast<int>(rtvDescriptorSize));
    }

    return backBuffers;
}

MW::ComPtr<ID3D12CommandAllocator> RendererSetup::CreateCommandAllocator(const MW::ComPtr<ID3D12Device2>& device, const D3D12_COMMAND_LIST_TYPE& type)
{
    MW::ComPtr<ID3D12CommandAllocator> commandAllocator;
    DX::ThrowIfFailed(device->CreateCommandAllocator(type, IID_PPV_ARGS(&commandAllocator)));

    return commandAllocator;
}

MW::ComPtr<ID3D12GraphicsCommandList> RendererSetup::CreateCommandList(const MW::ComPtr<ID3D12Device2>& device, const MW::ComPtr<ID3D12CommandAllocator>& commandAllocator, const D3D12_COMMAND_LIST_TYPE& type)
{
    MW::ComPtr<ID3D12GraphicsCommandList> commandList;
    DX::ThrowIfFailed(device->CreateCommandList(0, type, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));

    DX::ThrowIfFailed(commandList->Close());

    return commandList;
}

MW::ComPtr<ID3D12Fence> RendererSetup::CreateFence(const MW::ComPtr<ID3D12Device2>& device)
{
    MW::ComPtr<ID3D12Fence> fence;

    DX::ThrowIfFailed(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));

    return fence;
}

HANDLE RendererSetup::CreateFenceEvent()
{
    HANDLE fenceEvent = CreateEvent(nullptr, false, false, nullptr);

    assert(fenceEvent && "Failed to create fence event");

    return fenceEvent;
}

std::uint64_t RendererSetup::Signal(const MW::ComPtr<ID3D12CommandQueue>& commandQueue, const MW::ComPtr<ID3D12Fence>& fence, std::uint64_t& fenceValue)
{
    std::uint64_t fenceValueForSignal = ++fenceValue;

    DX::ThrowIfFailed(commandQueue->Signal(fence.Get(), fenceValueForSignal));

    return fenceValueForSignal;
}

void RendererSetup::WaitForFenceValue(const MW::ComPtr<ID3D12Fence>& fence, const std::uint64_t& fenceValue, const HANDLE& fenceEvent, std::chrono::milliseconds duration)
{
    if (fence->GetCompletedValue() < fenceValue)
    {
        DX::ThrowIfFailed(fence->SetEventOnCompletion(fenceValue, fenceEvent));
        WaitForSingleObject(fenceEvent, static_cast<DWORD>(duration.count()));
    }
}

void RendererSetup::Flush(const MW::ComPtr<ID3D12CommandQueue>& commandQueue, const MW::ComPtr<ID3D12Fence>& fence, const HANDLE& fenceEvent, std::uint64_t& fenceValue)
{
    std::uint64_t fenceValueForSignal = Signal(commandQueue, fence, fenceValue);
    WaitForFenceValue(fence, fenceValueForSignal, fenceEvent);
}


bool RendererSetup::CheckTearingSupport()
{
    bool allowTearing = false;

    MW::ComPtr<IDXGIFactory4> factory4;
    if (!SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory4))))
    {
        return false;
    }
    
    MW::ComPtr<IDXGIFactory5> factory5;
    if (!SUCCEEDED(factory4.As(&factory5)))
    {
        return false;
    }
    if
    (
        FAILED
        (
            factory5->CheckFeatureSupport
            (
                DXGI_FEATURE_PRESENT_ALLOW_TEARING, 
                &allowTearing,
                sizeof(allowTearing)
            )
        )
    )
    {
        allowTearing = false;
    }

    return allowTearing;
}
