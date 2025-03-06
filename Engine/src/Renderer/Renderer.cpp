#include "Renderer.hpp"

#include "DirectXHelper.hpp"
#include "RendererSetup.hpp"

Renderer::Renderer(const SDLWindow& window)
{
    #if defined(_DEBUG)
    m_debugInterface = RendererSetup::CreateDebugLayer();
    #endif

    // Standard DirectX12 Initialization
    m_device = RendererSetup::CreateDevice();
    m_commandQueue = RendererSetup::CreateCommandQueue(m_device, D3D12_COMMAND_LIST_TYPE_DIRECT);
    m_swapChain = RendererSetup::CreateSwapChain(window, m_commandQueue, 1280, 720, 3);
    m_backBufferDescriptorHeap = RendererSetup::CreateDescriptorHeap(m_device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, m_numFrames);
    m_backBuffers = RendererSetup::CreateBackBuffers(m_device, m_swapChain, m_backBufferDescriptorHeap, m_numFrames, m_rtvDescriptorSize);
    m_backBufferCommandAllocators =
    {
        RendererSetup::CreateCommandAllocator(m_device, D3D12_COMMAND_LIST_TYPE_DIRECT),
        RendererSetup::CreateCommandAllocator(m_device, D3D12_COMMAND_LIST_TYPE_DIRECT),
        RendererSetup::CreateCommandAllocator(m_device, D3D12_COMMAND_LIST_TYPE_DIRECT)
    };
    
    m_commandList = RendererSetup::CreateCommandList(m_device, m_backBufferCommandAllocators.at(0), D3D12_COMMAND_LIST_TYPE_DIRECT);
    m_fence = RendererSetup::CreateFence(m_device);
    m_fenceEvent = RendererSetup::CreateFenceEvent();

    m_imGuiSRVDescriptorHeap = RendererSetup::CreateDescriptorHeap(m_device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, m_numFrames);

    #if defined(_DEBUG)
    {
        ImGuiTool::Initialize(window, m_device, m_commandQueue, m_imGuiSRVDescriptorHeap, DXGI_FORMAT_R8G8B8A8_UNORM, 3);
    }
    #endif
}

Renderer::~Renderer()
{
    #if defined(_DEBUG)
    {
        ImGuiTool::Shutdown();
    }
    #endif
}

void Renderer::Draw()
{
    #if defined(_DEBUG)
    {
        ImGuiTool::Start();
        ImGuiTool::Run();
    }
    #endif
    
    Render();

    #if defined(_DEBUG)
    {
        ImGuiTool::End();
    }
    #endif

    const UINT syncInterval = m_VSync ? 1 : 0;
    const UINT presentFlags = RendererSetup::CheckTearingSupport() && !m_VSync ? DXGI_PRESENT_ALLOW_TEARING : 0;
    
    DX::ThrowIfFailed(m_swapChain->Present(syncInterval, presentFlags));
    
    m_frameFenceValues.at(m_backBufferIndex) = RendererSetup::Signal(m_commandQueue, m_fence, m_fenceValue);
    
    m_backBufferIndex = m_swapChain->GetCurrentBackBufferIndex();
    
    RendererSetup::WaitForFenceValue(m_fence, m_frameFenceValues.at(m_backBufferIndex), m_fenceEvent);
}


void Renderer::Render() const
{
    const MW::ComPtr<ID3D12CommandAllocator> currentCommandAllocator = m_backBufferCommandAllocators.at(m_backBufferIndex);
    const MW::ComPtr<ID3D12Resource> currentBackBuffer = m_backBuffers.at(m_backBufferIndex);

    static_cast<void>(currentCommandAllocator->Reset());
    static_cast<void>(m_commandList->Reset(currentCommandAllocator.Get(), nullptr));

    // Clear Render Target
    {
        CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(currentBackBuffer.Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    
        m_commandList->ResourceBarrier(1, &barrier);
    
        float clearColor[4] = { 0.6f, 0.9f, 0.5f, 1.0f };

        CD3DX12_CPU_DESCRIPTOR_HANDLE rtv = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_backBufferDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), m_backBufferIndex, m_rtvDescriptorSize);

        m_commandList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
        m_commandList->OMSetRenderTargets(1, &rtv, false, nullptr);

        std::vector<ID3D12DescriptorHeap*> descriptorHeaps =
        {
            m_imGuiSRVDescriptorHeap.Get()   
        };
        
        m_commandList->SetDescriptorHeaps(static_cast<std::uint32_t>(descriptorHeaps.size()), descriptorHeaps.data());
        ImGuiTool::RenderDrawData(m_commandList);
    }

    // Present
    {
        CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(currentBackBuffer.Get(),D3D12_RESOURCE_STATE_RENDER_TARGET,  D3D12_RESOURCE_STATE_PRESENT);

        m_commandList->ResourceBarrier(1, &barrier);

        DX::ThrowIfFailed(m_commandList->Close());

        std::vector<ID3D12CommandList*> commandLists =
        {
            m_commandList.Get()
        };

        m_commandQueue->ExecuteCommandLists(static_cast<std::uint32_t>(commandLists.size()), commandLists.data());
    }
}