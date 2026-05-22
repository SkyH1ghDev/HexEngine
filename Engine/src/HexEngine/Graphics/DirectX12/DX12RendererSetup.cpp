#include "DX12RendererSetup.hpp"

#include <HexEngine/Utils/DirectXUtils.hpp>
#include <dxgi1_6.h>
#include <SDL3/SDL.h>
#include <cassert>

namespace DXUtils = DirectXUtils;
using namespace HexEngine::Graphics::DirectX12;

#ifdef _DEBUG 
DebugLayer DX12RendererSetup::CreateDebugLayer()
{
    // Enables Debug Layer

    winrt::com_ptr<ID3D12Debug6> debugInterface;
    DXUtils::ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));
    debugInterface->EnableDebugLayer();
    return DebugLayer(debugInterface);
    
}
#endif

Device DX12RendererSetup::CreateDevice()
{
    // Create IDXGIFactory
    
    winrt::com_ptr<IDXGIFactory7> dxgiFactory;
    UINT createFactoryFlags = 0;
    
    #ifdef _DEBUG
    createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
    #endif
    
    DXUtils::ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory)));

    // Create Adapter For GPU Interaction

    winrt::com_ptr<IDXGIAdapter1> dxgiAdapter1;
    winrt::com_ptr<IDXGIAdapter4> dxgiAdapter4;
    std::size_t maxDedicatedVideoMemory = 0;

    for (std::uint32_t i = 0; dxgiFactory->EnumAdapters1(i, dxgiAdapter1.put()) != DXGI_ERROR_NOT_FOUND; ++i)
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

CommandQueue DX12RendererSetup::CreateCommandQueue(const Device& pDevice, const D3D12_COMMAND_LIST_TYPE pType)
{
    winrt::com_ptr<ID3D12CommandQueue> commandQueue;

    D3D12_COMMAND_QUEUE_DESC queueDesc =
    {
        .Type =     pType,
        .Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
        .Flags =    D3D12_COMMAND_QUEUE_FLAG_NONE,
        .NodeMask = 0
    };

    DXUtils::ThrowIfFailed(pDevice.GetCOM()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue)));

    return CommandQueue(commandQueue);
}

SwapChainManager DX12RendererSetup::CreateSwapChainManager(const HexEngine::SDL::SDLWindow& pWindow, const Device& pDevice, const CommandQueue& pCommandQueue, const uint8_t
                                                           pBufferCount)
{
    const SwapChain swapChain = CreateSwapChain(pWindow, pCommandQueue, pBufferCount);
    const DescriptorHeap descriptorHeap = CreateDescriptorHeap(pDevice, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, pBufferCount);
    const std::uint32_t rtvDescriptorSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    
    const std::vector<BackBuffer> backBuffers = CreateBackBuffers(pDevice, swapChain, descriptorHeap, rtvDescriptorSize, pBufferCount);
    
    return SwapChainManager(swapChain, descriptorHeap, backBuffers, rtvDescriptorSize);
}

SwapChain DX12RendererSetup::CreateSwapChain(const HexEngine::SDL::SDLWindow& pWindow, const CommandQueue& pCommandQueue, const std::uint32_t& pBufferCount)
{
    winrt::com_ptr<IDXGISwapChain4> swapChain;
    winrt::com_ptr<IDXGIFactory7> factory;

    UINT createFactoryFlags = 0;

    #ifdef _DEBUG
    createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
    #endif

    DXUtils::ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&factory)));

    std::uint32_t swapChainFlags = DX12DeviceCapabilities::CheckTearingSupport() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;
    
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc =
    {
        .Width =        pWindow.GetWidth(),
        .Height =       pWindow.GetHeight(),
        .Format =       DXGI_FORMAT_R8G8B8A8_UNORM,
        .Stereo =       false,
        .SampleDesc =
        {
            .Count      = 1,
            .Quality    = 0
        },
        .BufferUsage =  DXGI_USAGE_RENDER_TARGET_OUTPUT,
        .BufferCount =  pBufferCount,
        .Scaling =      DXGI_SCALING_STRETCH,
        .SwapEffect =   DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL,
        .AlphaMode =    DXGI_ALPHA_MODE_UNSPECIFIED,
        .Flags =        swapChainFlags
    };

    const HWND hWnd = static_cast<HWND>(SDL_GetPointerProperty(SDL_GetWindowProperties(pWindow.GetSDLWindow().lock().get()), SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr));
    
    winrt::com_ptr<IDXGISwapChain1> swapChain1;
    DXUtils::ThrowIfFailed
    (
        factory->CreateSwapChainForHwnd
        (
            pCommandQueue.GetCOM().get(),
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

DescriptorHeap DX12RendererSetup::CreateDescriptorHeap(const Device& pDevice, const D3D12_DESCRIPTOR_HEAP_TYPE& pType, const std::uint32_t& pNumDescriptors)
{
    winrt::com_ptr<ID3D12DescriptorHeap> descriptorHeap;

    const D3D12_DESCRIPTOR_HEAP_FLAGS flags = (pType == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE); 
    
    const D3D12_DESCRIPTOR_HEAP_DESC desc =
    {
        .Type = pType,
        .NumDescriptors = pNumDescriptors,
        .Flags = flags,
        .NodeMask = 0
    };

    DXUtils::ThrowIfFailed(pDevice.GetCOM()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptorHeap)));

    return DescriptorHeap(descriptorHeap);
}

std::vector<BackBuffer> DX12RendererSetup::CreateBackBuffers(const Device& pDevice, const SwapChain& pSwapChain, const DescriptorHeap& pDescriptorHeap, const uint64_t
                                                             pRenderTargetDescriptorSize, const uint8_t pBufferCount)
{
    
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(pDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
    
    std::vector<BackBuffer> backBuffers;
    
    for (int i = 0; i < pBufferCount; ++i)
    {
        /* --- THIS PART COULD PROBABLY BE REFACTORED INTO CreateResource / CreateRenderTarget, --- */
        /*     IDK HOW TO DO IT WELL YET THOUGH                                                     */
        winrt::com_ptr<ID3D12Resource2> backBufferResource;
        DXUtils::ThrowIfFailed(pSwapChain->GetBuffer(i, IID_PPV_ARGS(&backBufferResource)));

        Resource backBufferRenderTarget = Resource(backBufferResource);
        
        pDevice->CreateRenderTargetView(backBufferRenderTarget.GetRaw(), nullptr, rtvHandle);
        /* ---------------------------------------------------------------------------------------- */

        
        backBuffers.push_back(BackBuffer(backBufferRenderTarget));

        rtvHandle.Offset(static_cast<std::int32_t>(pRenderTargetDescriptorSize));
    }

    return backBuffers;
}

CommandAllocator DX12RendererSetup::CreateCommandAllocator(const Device& device, D3D12_COMMAND_LIST_TYPE type)
{
    winrt::com_ptr<ID3D12CommandAllocator> commandAllocator;
    DXUtils::ThrowIfFailed(device.GetCOM()->CreateCommandAllocator(type, IID_PPV_ARGS(&commandAllocator)));

    return CommandAllocator(commandAllocator);
}

CommandList DX12RendererSetup::CreateCommandList(const Device& device, const CommandAllocator& commandAllocator, D3D12_COMMAND_LIST_TYPE type)
{
    winrt::com_ptr<ID3D12GraphicsCommandList6> commandList;
    DXUtils::ThrowIfFailed(device->CreateCommandList(0, type, commandAllocator.GetRaw(), nullptr, IID_PPV_ARGS(&commandList)));

    return CommandList(commandList);
}

Fence DX12RendererSetup::CreateFence(const Device& device)
{
    winrt::com_ptr<ID3D12Fence1> fence;
    DXUtils::ThrowIfFailed(device.GetCOM()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));

    HANDLE fenceEvent = CreateFenceEvent();
    
    return Fence(fence, fenceEvent);
}

HANDLE DX12RendererSetup::CreateFenceEvent()
{
    HANDLE fenceEvent = CreateEvent(nullptr, false, false, nullptr);

    assert(fenceEvent && "Failed to create fence event");

    return fenceEvent;
}

void DX12RendererSetup::WaitForFenceValue(const winrt::com_ptr<ID3D12Fence>& fence, const std::uint64_t& fenceValue, const HANDLE& fenceEvent, std::chrono::milliseconds duration)
{
    if (fence->GetCompletedValue() < fenceValue)
    {
        DXUtils::ThrowIfFailed(fence->SetEventOnCompletion(fenceValue, fenceEvent));
        WaitForSingleObject(fenceEvent, static_cast<DWORD>(duration.count()));
    }
}