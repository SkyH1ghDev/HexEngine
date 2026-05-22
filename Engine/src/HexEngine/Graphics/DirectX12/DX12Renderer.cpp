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
    {
        m_debugInterface = DX12RendererSetup::CreateDebugLayer();
    }
    #endif

    // Standard DirectX12 Initialization
    m_device = DX12RendererSetup::CreateDevice();
    m_commandQueue = DX12RendererSetup::CreateCommandQueue(m_device, D3D12_COMMAND_LIST_TYPE_DIRECT);
    m_swapChainManager = DX12RendererSetup::CreateSwapChainManager(window, m_device, m_commandQueue, m_numFrames);

    BackBuffer backBuffer = m_swapChainManager.GetBackBufferAt(0);
    CommandAllocator commandAllocator = backBuffer.GetCommandAllocator();
    
    m_commandList = DX12RendererSetup::CreateCommandList(m_device, commandAllocator, D3D12_COMMAND_LIST_TYPE_DIRECT);
    m_fence = DX12RendererSetup::CreateFence(m_device);

    #if defined(_DEBUG)
    {
        m_imGuiDescriptorHeap = DX12RendererSetup::CreateDescriptorHeap(m_device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, m_numFrames);
        HexEngine::Graphics::UI::ImGuiTool::Initialize(window, m_device, m_commandQueue, m_imGuiDescriptorHeap, DXGI_FORMAT_R8G8B8A8_UNORM, 3);
    }
    #endif

	// Load Assets
    {
		// Load Meshes
        mMeshMaxwell = Assets::MeshLoader::LoadMesh("../../Engine/assets/Meshes/Maxwell.obj");
        Assets::MeshLoader::UploadMeshResources(m_device, m_commandQueue, commandAllocator, m_commandList, mMeshMaxwell);

        mMeshWhiskers = Assets::MeshLoader::LoadMesh("../../Engine/assets/Meshes/Whiskers.obj");
        Assets::MeshLoader::UploadMeshResources(m_device, m_commandQueue, commandAllocator, m_commandList, mMeshWhiskers);

		// Load Shaders
        //mVShader = Assets::ShaderLoader::LoadShader("../../Build/target/Shader/cso/vs_VertexShader.cso");
        mMShader = Assets::ShaderLoader::LoadShader("../../Build/target/Shader/cso/ms_MeshShader.cso");
        mPShader = Assets::ShaderLoader::LoadShader("../../Build/target/Shader/cso/ps_PixelShader.cso");

		// Create Root Signatures
		mMeshRootSignature = Assets::ShaderLoader::CreateRootSignature(m_device, mMShader);
		//mGraphicsRootSignature = Assets::ShaderLoader::CreateRootSignature(m_device, mVShader);

		// Create Pipeline States
		mMeshPipelineState = Assets::ShaderLoader::CreateMeshPipelineState(m_device, mMeshRootSignature, nullptr, &mMShader, &mPShader);
		//mGraphicsPipelineState = Assets::ShaderLoader::CreateGraphicsPipelineState(m_device, mGraphicsRootSignature, Assets::MeshLoader::c_DefualtElementDesc, 3, &mVShader, &mPShader);
    }
}

DX12Renderer::~DX12Renderer()
{
    #if defined(_DEBUG)
    {
        HexEngine::Graphics::UI::ImGuiTool::Shutdown();
    }
    #endif
    
    std::vector<BackBuffer> backBuffers = m_swapChainManager.GetBackBuffers();

    for (BackBuffer backBuffer : backBuffers)
    {
        m_fence.Flush(backBuffer.GetFenceValue());
    }
}

void DX12Renderer::Draw()
{
    #if defined(_DEBUG)
    {
        HexEngine::Graphics::UI::ImGuiTool::Start();
        HexEngine::Graphics::UI::ImGuiTool::Run();
    }
    #endif
    
    Render();

    #if defined(_DEBUG)
    {
        HexEngine::Graphics::UI::ImGuiTool::End();
    }
    #endif

    m_swapChainManager.PresentFrame(m_vSync);

    BackBuffer& backBuffer = m_swapChainManager.GetCurrentBackBuffer();

    std::uint64_t fenceValue = backBuffer.GetFenceValue();
    fenceValue = m_commandQueue.Signal(m_fence, fenceValue);
    backBuffer.SetFenceValue(fenceValue);
    
    m_swapChainManager.UpdateBackBufferIndex();

    m_fence.WaitForValue(backBuffer.GetFenceValue());   
}

void DX12Renderer::Render()
{
    BackBuffer &backBuffer = m_swapChainManager.GetCurrentBackBuffer();
    DescriptorHeap &backBufferDescriptorHeap = m_swapChainManager.GetDescriptorHeap();
    CommandAllocator &backBufferCommandAllocator = backBuffer.GetCommandAllocator();
    Resource &backBufferRenderTarget = backBuffer.GetRenderTarget();

    HRESULT res = backBufferCommandAllocator->Reset();
    if (FAILED(res))
    {
        std::print("Failed to reset backBufferCommandAllocator");
    }
    
    res = m_commandList->Reset(backBufferCommandAllocator.GetRaw(), nullptr);
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
    
        m_commandList->ResourceBarrier(1, &barrier);
    
        constexpr std::array<float, 4> clearColour = { 0.6f, 0.9f, 0.5f, 1.0f };

        const CD3DX12_CPU_DESCRIPTOR_HANDLE rtv = CD3DX12_CPU_DESCRIPTOR_HANDLE(
            backBufferDescriptorHeap.GetCOM()->GetCPUDescriptorHandleForHeapStart(), 
            m_swapChainManager.GetCurrentBackBufferIndex(), 
            m_swapChainManager.GetRenderTargetDescriptorSize()
        );

        m_commandList->ClearRenderTargetView(rtv, clearColour.data(), 0, nullptr);
        m_commandList->OMSetRenderTargets(1, &rtv, false, nullptr);

        std::vector<ID3D12DescriptorHeap*> descriptorHeaps =
        {
            #if defined(_DEBUG)
            m_imGuiDescriptorHeap.GetRaw()
            #endif
        };
        
        m_commandList->SetDescriptorHeaps(static_cast<std::uint32_t>(descriptorHeaps.size()), descriptorHeaps.data());
        #if defined(_DEBUG)
        HexEngine::Graphics::UI::ImGuiTool::RenderDrawData(m_commandList);
        #endif
    }

    // Present
    {
        CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
            backBufferRenderTarget.GetRaw(),
            D3D12_RESOURCE_STATE_RENDER_TARGET,  
            D3D12_RESOURCE_STATE_PRESENT
        );

        m_commandList->ResourceBarrier(1, &barrier);

        DirectXUtils::ThrowIfFailed(m_commandList->Close());

        m_commandQueue.AppendCommandList(m_commandList);
        m_commandQueue.ExecuteCommandLists();
    }
}