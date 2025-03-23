#include "Renderer.hpp"

#include <HexEngine/Renderer/ImGui/ImGuiTool.hpp>
#include <HexEngine/Renderer/DirectX/DirectXUtils.hpp>
#include <HexEngine/Renderer/RendererSetup.hpp>

Renderer::Renderer(const SDLWindow& window)
{
    #if defined(_DEBUG)
    {
        m_debugInterface = RendererSetup::CreateDebugLayer();
    }
    #endif

    // Standard DirectX12 Initialization
    m_device = RendererSetup::CreateDevice();
    m_commandQueue = RendererSetup::CreateCommandQueue(m_device, D3D12_COMMAND_LIST_TYPE_DIRECT);
    m_swapChainManager = RendererSetup::CreateSwapChainManager(window, m_device, m_commandQueue, m_numFrames);

    BackBuffer backBuffer = m_swapChainManager.GetBackBufferAt(0);
    CommandAllocator commandAllocator = backBuffer.GetCommandAllocator();
    
    m_commandList = RendererSetup::CreateCommandList(m_device, commandAllocator, D3D12_COMMAND_LIST_TYPE_DIRECT);
    m_fence = RendererSetup::CreateFence(m_device);

    #if defined(_DEBUG)
    {
        m_imGuiDescriptorHeap = RendererSetup::CreateDescriptorHeap(m_device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, m_numFrames);
        ImGuiTool::Initialize(window, m_device, m_commandQueue, m_imGuiDescriptorHeap, DXGI_FORMAT_R8G8B8A8_UNORM, 3);
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

    m_swapChainManager.PresentFrame(m_vSync);

    BackBuffer backBuffer = m_swapChainManager.GetCurrentBackBuffer();

    std::uint64_t fenceValue = backBuffer.GetFenceValue();
    fenceValue = m_commandQueue.Signal(m_fence, fenceValue);
    backBuffer.SetFenceValue(fenceValue);

    m_swapChainManager.UpdateBackBufferIndex();

    backBuffer = m_swapChainManager.GetCurrentBackBuffer();
    m_fence.WaitForValue(backBuffer.GetFenceValue());   
}


void Renderer::Render()
{
    const BackBuffer backBuffer = m_swapChainManager.GetCurrentBackBuffer();
    const DescriptorHeap backBufferDescriptorHeap = m_swapChainManager.GetDescriptorHeap();
    const CommandAllocator backBufferCommandAllocator = backBuffer.GetCommandAllocator();
    const Resource backBufferRenderTarget = backBuffer.GetRenderTarget();

    static_cast<void>(backBufferCommandAllocator->Reset());
    static_cast<void>(m_commandList->Reset(backBufferCommandAllocator.GetRaw(), nullptr));

    // Clear Render Target
    {
        const CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(backBufferRenderTarget.GetRaw(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    
        m_commandList->ResourceBarrier(1, &barrier);
    
        constexpr std::array<float, 4> clearColour = { 0.6f, 0.9f, 0.5f, 1.0f };

        const CD3DX12_CPU_DESCRIPTOR_HANDLE rtv = CD3DX12_CPU_DESCRIPTOR_HANDLE(backBufferDescriptorHeap.GetCOM()->GetCPUDescriptorHandleForHeapStart(), m_swapChainManager.GetCurrentBackBufferIndex(), m_swapChainManager.GetRenderTargetDescriptorSize());

        m_commandList->ClearRenderTargetView(rtv, clearColour.data(), 0, nullptr);
        m_commandList->OMSetRenderTargets(1, &rtv, false, nullptr);

        std::vector<ID3D12DescriptorHeap*> descriptorHeaps =
        {
            m_imGuiDescriptorHeap.GetRaw()
        };
        
        m_commandList->SetDescriptorHeaps(static_cast<std::uint32_t>(descriptorHeaps.size()), descriptorHeaps.data());
        ImGuiTool::RenderDrawData(m_commandList);
    }

    // Present
    {
        CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(backBufferRenderTarget.GetRaw(),D3D12_RESOURCE_STATE_RENDER_TARGET,  D3D12_RESOURCE_STATE_PRESENT);

        m_commandList->ResourceBarrier(1, &barrier);

        DirectXUtils::ThrowIfFailed(m_commandList->Close());

        m_commandQueue.AppendCommandList(m_commandList);
        m_commandQueue.ExecuteCommandLists();
    }
}