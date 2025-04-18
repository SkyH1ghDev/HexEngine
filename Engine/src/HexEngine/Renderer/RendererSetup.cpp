#include "RendererSetup.hpp"

#include <HexEngine/Renderer/DirectX/DirectXUtils.hpp>
#include <dxgi1_6.h>
#include <SDL3/SDL.h>
#include <cassert>

namespace DXUtils = DirectXUtils;

#if defined(_DEBUG) 
DebugLayer RendererSetup::CreateDebugLayer()
{
    // Enables Debug Layer

    winrt::com_ptr<ID3D12Debug6> debugInterface;
    DXUtils::ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));
    debugInterface->EnableDebugLayer();
    return DebugLayer(debugInterface);
    
}
#endif

Device RendererSetup::CreateDevice()
{
    // Create IDXGIFactory
    
    winrt::com_ptr<IDXGIFactory7> dxgiFactory;
    UINT createFactoryFlags = 0;
    
    #if defined(_DEBUG)
        createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
    #endif
    
    DXUtils::ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory)));

    // Create Adapter For GPU Interaction

    winrt::com_ptr<IDXGIAdapter1> dxgiAdapter1;
    winrt::com_ptr<IDXGIAdapter4> dxgiAdapter4;
    SIZE_T maxDedicatedVideoMemory = 0;

    for (UINT i = 0; dxgiFactory->EnumAdapters1(i, dxgiAdapter1.put()) != DXGI_ERROR_NOT_FOUND; ++i)
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
                    dxgiAdapter1.get(),
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
            dxgiAdapter1.as(dxgiAdapter4);
        }
    }
    
    // Create Device

    winrt::com_ptr<ID3D12Device9> device;
    DXUtils::ThrowIfFailed(D3D12CreateDevice(dxgiAdapter1.get(), D3D_FEATURE_LEVEL_12_2, IID_PPV_ARGS(&device)));

    #ifdef _DEBUG
        
    winrt::com_ptr<ID3D12InfoQueue> pInfoQueue;
    device.as(pInfoQueue);
    
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

    D3D12_INFO_QUEUE_FILTER filter =
    {
        .AllowList =
        {
            .NumCategories =    0,
            .pCategoryList =    nullptr,
            .NumSeverities =    0,
            .pSeverityList =    nullptr,
            .NumIDs =           0,
            .pIDList =          nullptr
        },
        
        .DenyList =
        {
            .NumCategories =    0,
            .pCategoryList =    nullptr,
            .NumSeverities =    _countof(severities),
            .pSeverityList =    severities,
            .NumIDs =           _countof(denyIds),
            .pIDList =          denyIds
        }
    };

    DXUtils::ThrowIfFailed(pInfoQueue->PushStorageFilter(&filter));
    
    #endif
    
    return Device(device);
}

CommandQueue RendererSetup::CreateCommandQueue(const Device& device, D3D12_COMMAND_LIST_TYPE type)
{
    winrt::com_ptr<ID3D12CommandQueue> commandQueue;

    D3D12_COMMAND_QUEUE_DESC queueDesc =
    {
        .Type =     type,
        .Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
        .Flags =    D3D12_COMMAND_QUEUE_FLAG_NONE,
        .NodeMask = 0
    };

    DXUtils::ThrowIfFailed(device.GetCOM()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue)));

    return CommandQueue(commandQueue);
}

SwapChainManager RendererSetup::CreateSwapChainManager(const SDLWindow& window, const Device& device, const CommandQueue& commandQueue, std::uint8_t bufferCount)
{
    // TODO: UNHARD-CODE THIS
    constexpr std::uint32_t width = 1280, height = 720;
    
    const SwapChain swapChain = CreateSwapChain(window, commandQueue, width, height, bufferCount);
    const DescriptorHeap descriptorHeap = CreateDescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, bufferCount);
    const std::uint32_t rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    
    const std::vector<BackBuffer> backBuffers = CreateBackBuffers(device, swapChain, descriptorHeap, rtvDescriptorSize, bufferCount);
    
    return SwapChainManager(swapChain, descriptorHeap, backBuffers, rtvDescriptorSize);
}

SwapChain RendererSetup::CreateSwapChain(const SDLWindow& window, const CommandQueue& commandQueue, const std::uint32_t& width, const std::uint32_t& height, const std::uint32_t& bufferCount)
{
    winrt::com_ptr<IDXGISwapChain4> swapChain;
    winrt::com_ptr<IDXGIFactory7> factory;

    UINT createFactoryFlags = 0;

    #ifdef _DEBUG
    createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
    #endif

    DXUtils::ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&factory)));

    std::uint32_t swapChainFlags = DXUtils::CheckTearingSupport() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;
    
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc =
    {
        .Width =        width,
        .Height =       height,
        .Format =       DXGI_FORMAT_R8G8B8A8_UNORM,
        .Stereo =       false,
        .SampleDesc =
        {
            .Count      = 1,
            .Quality    = 0
        },
        .BufferUsage =  DXGI_USAGE_RENDER_TARGET_OUTPUT,
        .BufferCount =  bufferCount,
        .Scaling =      DXGI_SCALING_STRETCH,
        .SwapEffect =   DXGI_SWAP_EFFECT_FLIP_DISCARD,
        .AlphaMode =    DXGI_ALPHA_MODE_UNSPECIFIED,
        .Flags =        swapChainFlags
    };

    const HWND hWnd = static_cast<HWND>(SDL_GetPointerProperty(SDL_GetWindowProperties(window.GetSDLWindow()), SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr));
    
    winrt::com_ptr<IDXGISwapChain1> swapChain1;
    DXUtils::ThrowIfFailed
    (
        factory->CreateSwapChainForHwnd
        (
            commandQueue.GetCOM().get(),
            hWnd,
            &swapChainDesc,
            nullptr,
            nullptr,
            swapChain1.put()
        )
    );

    DXUtils::ThrowIfFailed(factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));
    swapChain1.as(swapChain);

    return SwapChain(swapChain);
}

DescriptorHeap RendererSetup::CreateDescriptorHeap(const Device& device, const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint32_t& numDescriptors)
{
    winrt::com_ptr<ID3D12DescriptorHeap> descriptorHeap;

    const D3D12_DESCRIPTOR_HEAP_FLAGS flags = (type == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE); 
    
    const D3D12_DESCRIPTOR_HEAP_DESC desc =
    {
        .Type = type,
        .NumDescriptors = numDescriptors,
        .Flags = flags,
        .NodeMask = 0
    };

    DXUtils::ThrowIfFailed(device.GetCOM()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptorHeap)));

    return DescriptorHeap(descriptorHeap);
}

std::vector<BackBuffer> RendererSetup::CreateBackBuffers(const Device& device, const SwapChain& swapChain, const DescriptorHeap& descriptorHeap, std::uint64_t renderTargetDescriptorSize, std::uint8_t bufferCount)
{
    
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(descriptorHeap->GetCPUDescriptorHandleForHeapStart());
    
    std::vector<BackBuffer> backBuffers;
    
    for (int i = 0; i < bufferCount; ++i)
    {
        /* --- THIS PART COULD PROBABLY BE REFACTORED INTO CreateResource / CreateRenderTarget, --- */
        /*     IDK HOW TO DO IT WELL YET THOUGH                                                     */
        winrt::com_ptr<ID3D12Resource2> backBufferResource;
        DXUtils::ThrowIfFailed(swapChain->GetBuffer(i, IID_PPV_ARGS(&backBufferResource)));

        Resource backBufferRenderTarget = Resource(backBufferResource);
        
        device->CreateRenderTargetView(backBufferRenderTarget.GetRaw(), nullptr, rtvHandle);
        /* ---------------------------------------------------------------------------------------- */

        CommandAllocator commandAllocator = CreateCommandAllocator(device, D3D12_COMMAND_LIST_TYPE_DIRECT);
        
        backBuffers.push_back(BackBuffer(backBufferRenderTarget, commandAllocator));

        rtvHandle.Offset(static_cast<std::int32_t>(renderTargetDescriptorSize));
    }

    return backBuffers;
}

CommandAllocator RendererSetup::CreateCommandAllocator(const Device& device, D3D12_COMMAND_LIST_TYPE type)
{
    winrt::com_ptr<ID3D12CommandAllocator> commandAllocator;
    DXUtils::ThrowIfFailed(device.GetCOM()->CreateCommandAllocator(type, IID_PPV_ARGS(&commandAllocator)));

    return CommandAllocator(commandAllocator);
}

CommandList RendererSetup::CreateCommandList(const Device& device, const CommandAllocator& commandAllocator, D3D12_COMMAND_LIST_TYPE type)
{
    winrt::com_ptr<ID3D12GraphicsCommandList6> commandList;
    DXUtils::ThrowIfFailed(device.GetCOM()->CreateCommandList(0, type, commandAllocator.GetCOM().get(), nullptr, IID_PPV_ARGS(&commandList)));

    DXUtils::ThrowIfFailed(commandList->Close());

    return CommandList(commandList);
}

Fence RendererSetup::CreateFence(const Device& device)
{
    winrt::com_ptr<ID3D12Fence1> fence;
    DXUtils::ThrowIfFailed(device.GetCOM()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));

    HANDLE fenceEvent = CreateFenceEvent();
    
    return Fence(fence, fenceEvent);
}

HANDLE RendererSetup::CreateFenceEvent()
{
    HANDLE fenceEvent = CreateEvent(nullptr, false, false, nullptr);

    assert(fenceEvent && "Failed to create fence event");

    return fenceEvent;
}

std::uint64_t RendererSetup::Signal(const winrt::com_ptr<ID3D12CommandQueue>& commandQueue, const winrt::com_ptr<ID3D12Fence>& fence, std::uint64_t& fenceValue)
{
    std::uint64_t fenceValueForSignal = ++fenceValue;

    DXUtils::ThrowIfFailed(commandQueue->Signal(fence.get(), fenceValueForSignal));

    return fenceValueForSignal;
}

void RendererSetup::WaitForFenceValue(const winrt::com_ptr<ID3D12Fence>& fence, const std::uint64_t& fenceValue, const HANDLE& fenceEvent, std::chrono::milliseconds duration)
{
    if (fence->GetCompletedValue() < fenceValue)
    {
        DXUtils::ThrowIfFailed(fence->SetEventOnCompletion(fenceValue, fenceEvent));
        WaitForSingleObject(fenceEvent, static_cast<DWORD>(duration.count()));
    }
}

void RendererSetup::Flush(const winrt::com_ptr<ID3D12CommandQueue>& commandQueue, const winrt::com_ptr<ID3D12Fence>& fence, const HANDLE& fenceEvent, std::uint64_t& fenceValue)
{
    std::uint64_t fenceValueForSignal = Signal(commandQueue, fence, fenceValue);
    WaitForFenceValue(fence, fenceValueForSignal, fenceEvent);
}