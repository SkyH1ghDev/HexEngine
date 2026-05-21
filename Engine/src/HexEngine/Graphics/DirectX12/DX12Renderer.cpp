#include "DX12Renderer.hpp"

#include <print>
#include <HexEngine/Utils/DirectXUtils.hpp>
#include <HexEngine/Graphics/DirectX12/DX12RendererSetup.hpp>

#if defined(_DEBUG)
#include <HexEngine/Graphics/UI/ImGuiTool.hpp>
#endif

using namespace HexEngine::Graphics::DirectX12;

DX12Renderer::DX12Renderer(const HexEngine::SDL::SDLWindow& window)
{
    #if defined(_DEBUG)
    mDebugInterface = DX12RendererSetup::CreateDebugLayer();
    #endif

    // Standard DirectX12 Initialization
    mDevice = DX12RendererSetup::CreateDevice();
    
    if (!DX12DeviceCapabilities::CheckMeshShaderSupport(mDevice))
    {
        throw std::runtime_error("Mesh Shader support is required but not available on this device.");
    }
    
    mDirectCommandQueue = DX12RendererSetup::CreateCommandQueue(mDevice, D3D12_COMMAND_LIST_TYPE_DIRECT);
    mCopyCommandQueue = DX12RendererSetup::CreateCommandQueue(mDevice, D3D12_COMMAND_LIST_TYPE_COPY);
    mComputeCommandQueue = DX12RendererSetup::CreateCommandQueue(mDevice, D3D12_COMMAND_LIST_TYPE_COMPUTE);
    
    mSwapChainManager = DX12RendererSetup::CreateSwapChainManager(window, mDevice, mDirectCommandQueue, mNumFrames);

    BackBuffer backBuffer = mSwapChainManager.GetBackBufferAt(0);
    CommandAllocator commandAllocator = backBuffer.GetCommandAllocator();
    
    mCommandList = DX12RendererSetup::CreateCommandList(mDevice, commandAllocator, D3D12_COMMAND_LIST_TYPE_DIRECT);
    mFence = DX12RendererSetup::CreateFence(mDevice);

    #if defined(_DEBUG)
    mImGuiDescriptorHeap = DX12RendererSetup::CreateDescriptorHeap(mDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, mNumFrames);
    HexEngine::Graphics::UI::ImGuiTool::Initialize(window, mDevice, mDirectCommandQueue, mImGuiDescriptorHeap, DXGI_FORMAT_R8G8B8A8_UNORM, 3);
    #endif
}

DX12Renderer::~DX12Renderer()
{
    #if defined(_DEBUG)
    HexEngine::Graphics::UI::ImGuiTool::Shutdown();
    #endif
    
    std::vector<BackBuffer> backBuffers = mSwapChainManager.GetBackBuffers();

    for (BackBuffer backBuffer : backBuffers)
    {
        mFence.Flush(backBuffer.GetFenceValue());
    }
}

void DX12Renderer::Draw()
{
    #if defined(_DEBUG)
    HexEngine::Graphics::UI::ImGuiTool::Start();
    HexEngine::Graphics::UI::ImGuiTool::Run();
    #endif
    
    Render();

    #if defined(_DEBUG)
    HexEngine::Graphics::UI::ImGuiTool::End();
    #endif

    mSwapChainManager.PresentFrame(mVSync);

    BackBuffer& backBuffer = mSwapChainManager.GetCurrentBackBuffer();

    std::uint64_t fenceValue = backBuffer.GetFenceValue();
    fenceValue = mDirectCommandQueue.GPUSignal(mFence, fenceValue);
    backBuffer.SetFenceValue(fenceValue);
    
    mSwapChainManager.UpdateBackBufferIndex();

    mFence.WaitForValue(backBuffer.GetFenceValue());   
}

void DX12Renderer::Render()
{
    
    
    // CREATE VERTEX BUFFER
    CD3DX12_HEAP_PROPERTIES vertexBufferHeapProps
    {
        .type = D3D12_HEAP_TYPE_DEFAULT
    };
    
    CD3DX12_RESOURCE_DESC::Buffer() 
        
    static_cast<void>(
        mDevice->CreateCommittedResource(
            vertexBufferHeapProps, D3D12_HEAP_FLAG_NONE,
            
        )
    );
    winrt::com_ptr<ID3D12Resource> vertexBuffer = mCommandList->CopyVertexBuffer();
    
    
    BackBuffer& backBuffer = mSwapChainManager.GetCurrentBackBuffer();
    DescriptorHeap& backBufferDescriptorHeap = mSwapChainManager.GetDescriptorHeap();
    CommandAllocator& backBufferCommandAllocator = backBuffer.GetCommandAllocator();
    Resource& backBufferRenderTarget = backBuffer.GetRenderTarget();
    
    HRESULT res = backBufferCommandAllocator->Reset();
    if (FAILED(res))
    {
        std::print("Failed to reset backBufferCommandAllocator");
    }
    
    res = mCommandList->Reset(backBufferCommandAllocator.GetRaw(), nullptr);
    if (FAILED(res))
    {
        std::print("Failed to reset commandList");
    }
    

    // Clear Render Target
    {
        const CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
            backBufferRenderTarget.GetRaw(), 
            D3D12_RESOURCE_STATE_PRESENT, 
            D3D12_RESOURCE_STATE_RENDER_TARGET
        );
    
        mCommandList->ResourceBarrier(1, &barrier);
    
        constexpr std::array<float, 4> clearColour = { 0.6f, 0.9f, 0.5f, 1.0f };

        const CD3DX12_CPU_DESCRIPTOR_HANDLE rtv = CD3DX12_CPU_DESCRIPTOR_HANDLE(
            backBufferDescriptorHeap.GetCOM()->GetCPUDescriptorHandleForHeapStart(), 
            mSwapChainManager.GetCurrentBackBufferIndex(), 
            mSwapChainManager.GetRenderTargetDescriptorSize()
        );

        mCommandList->ClearRenderTargetView(rtv, clearColour.data(), 0, nullptr);
        mCommandList->OMSetRenderTargets(1, &rtv, false, nullptr);

        std::vector<ID3D12DescriptorHeap*> descriptorHeaps =
        {
            #if defined(_DEBUG)
            mImGuiDescriptorHeap.GetRaw()
            #endif
        };
        
        mCommandList->SetDescriptorHeaps(static_cast<std::uint32_t>(descriptorHeaps.size()), descriptorHeaps.data());
        #if defined(_DEBUG)
        HexEngine::Graphics::UI::ImGuiTool::RenderDrawData(mCommandList);
        #endif
    }

    // Present
    {
        CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
            backBufferRenderTarget.GetRaw(),
            D3D12_RESOURCE_STATE_RENDER_TARGET,  
            D3D12_RESOURCE_STATE_PRESENT
        );

        mCommandList->ResourceBarrier(1, &barrier);

        DirectXUtils::ThrowIfFailed(mCommandList->Close());

        // TODO: EXECUTE COMMAND LISTS :)
        
        //mCommandQueue.ExecuteCommandLists({mCommandList});
    }
}