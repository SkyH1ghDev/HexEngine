#include "DX12Renderer.hpp"

#include <DirectXMesh.h>
#include <print>
#include <HexEngine/Utils/DirectXUtils.hpp>
#include <HexEngine/Graphics/DirectX12/DX12RendererSetup.hpp>

#if defined(_DEBUG)
#include <HexEngine/Graphics/UI/ImGuiTool.hpp>
#endif

using namespace HexEngine::Graphics::DirectX12;

struct VertexPosColor
{
    DirectX::XMFLOAT3 Position;
    DirectX::XMFLOAT3 Color;
};

static VertexPosColor g_Vertices[8] = {
    { DirectX::XMFLOAT3( -1.0f, -1.0f, -1.0f ), DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f ) },  // 0
    { DirectX::XMFLOAT3( -1.0f, 1.0f, -1.0f ), DirectX::XMFLOAT3( 0.0f, 1.0f, 0.0f ) },   // 1
    { DirectX::XMFLOAT3( 1.0f, 1.0f, -1.0f ), DirectX::XMFLOAT3( 1.0f, 1.0f, 0.0f ) },    // 2
    { DirectX::XMFLOAT3( 1.0f, -1.0f, -1.0f ), DirectX::XMFLOAT3( 1.0f, 0.0f, 0.0f ) },   // 3
    { DirectX::XMFLOAT3( -1.0f, -1.0f, 1.0f ), DirectX::XMFLOAT3( 0.0f, 0.0f, 1.0f ) },   // 4
    { DirectX::XMFLOAT3( -1.0f, 1.0f, 1.0f ), DirectX::XMFLOAT3( 0.0f, 1.0f, 1.0f ) },    // 5
    { DirectX::XMFLOAT3( 1.0f, 1.0f, 1.0f ), DirectX::XMFLOAT3( 1.0f, 1.0f, 1.0f ) },     // 6
    { DirectX::XMFLOAT3( 1.0f, -1.0f, 1.0f ), DirectX::XMFLOAT3( 1.0f, 0.0f, 1.0f ) }     // 7
};

static WORD g_Indices[36] = { 0, 1, 2, 0, 2, 3, 4, 6, 5, 4, 7, 6, 4, 5, 1, 4, 1, 0,
                               3, 2, 6, 3, 6, 7, 1, 5, 6, 1, 6, 2, 4, 0, 3, 4, 3, 7 };

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
    
    mCopyCommandQueue = DX12RendererSetup::CreateCommandQueue(mDevice, D3D12_COMMAND_LIST_TYPE_COPY);
    mCopyCommandAllocator = DX12RendererSetup::CreateCommandAllocator(mDevice, D3D12_COMMAND_LIST_TYPE_COPY);
    mCopyCommandList = DX12RendererSetup::CreateCommandList(mDevice, mCopyCommandAllocator, D3D12_COMMAND_LIST_TYPE_COPY);
    
    mDirectCommandQueue = DX12RendererSetup::CreateCommandQueue(mDevice, D3D12_COMMAND_LIST_TYPE_DIRECT);
    mDirectCommandAllocator = DX12RendererSetup::CreateCommandAllocator(mDevice, D3D12_COMMAND_LIST_TYPE_DIRECT);
    mDirectCommandList = DX12RendererSetup::CreateCommandList(mDevice, mDirectCommandAllocator, D3D12_COMMAND_LIST_TYPE_DIRECT);
    
    mComputeCommandQueue = DX12RendererSetup::CreateCommandQueue(mDevice, D3D12_COMMAND_LIST_TYPE_COMPUTE);
    mComputeCommandAllocator = DX12RendererSetup::CreateCommandAllocator(mDevice, D3D12_COMMAND_LIST_TYPE_COMPUTE);
    mComputeCommandList = DX12RendererSetup::CreateCommandList(mDevice, mComputeCommandAllocator, D3D12_COMMAND_LIST_TYPE_COMPUTE);
    
    mSwapChainManager = DX12RendererSetup::CreateSwapChainManager(window, mDevice, mDirectCommandQueue, mNumFrames);
    mFence = DX12RendererSetup::CreateFence(mDevice);

    #if defined(_DEBUG)
    mImGuiDescriptorHeap = DX12RendererSetup::CreateDescriptorHeap(mDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, mNumFrames);
    HexEngine::Graphics::UI::ImGuiTool::Initialize(window, mDevice, mDirectCommandQueue, mImGuiDescriptorHeap, DXGI_FORMAT_R8G8B8A8_UNORM, 3);
    #endif
    
    if (!DX12DeviceCapabilities::CheckMeshShaderSupport(mDevice))
    {
        throw std::runtime_error("Mesh Shader support is required but not available on this device.");
    }
    
    mDirectCommandList->Close();
    
	// Load Assets
    {
		// Load Meshes
        mMeshMaxwell = Assets::MeshLoader::LoadMesh("../../Engine/assets/Meshes/Maxwell.obj");
        Assets::MeshLoader::UploadMeshResources(mDevice, mDirectCommandQueue, mDirectCommandAllocator, mDirectCommandList, mMeshMaxwell);

        mMeshWhiskers = Assets::MeshLoader::LoadMesh("../../Engine/assets/Meshes/Whiskers.obj");
        Assets::MeshLoader::UploadMeshResources(mDevice, mDirectCommandQueue, mDirectCommandAllocator, mDirectCommandList, mMeshWhiskers);

		// Load Shaders
        //mVShader = Assets::ShaderLoader::LoadShader("../../Build/target/Shader/cso/vs_VertexShader.cso");
        mMShader = Assets::ShaderLoader::LoadShader("../../Build/target/Shader/cso/ms_MeshShader.cso");
        mPShader = Assets::ShaderLoader::LoadShader("../../Build/target/Shader/cso/ps_PixelShader.cso");

		// Create Root Signatures
		mMeshRootSignature = Assets::ShaderLoader::CreateRootSignature(mDevice, mMShader);
		//mGraphicsRootSignature = Assets::ShaderLoader::CreateRootSignature(m_device, mVShader);

		// Create Pipeline States
		mMeshPipelineState = Assets::ShaderLoader::CreateMeshPipelineState(mDevice, mMeshRootSignature, nullptr, &mMShader, &mPShader);
		//mGraphicsPipelineState = Assets::ShaderLoader::CreateGraphicsPipelineState(m_device, mGraphicsRootSignature, Assets::MeshLoader::c_DefualtElementDesc, 3, &mVShader, &mPShader);
    }
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
    BackBuffer &backBuffer = mSwapChainManager.GetCurrentBackBuffer();
    DescriptorHeap &backBufferDescriptorHeap = mSwapChainManager.GetDescriptorHeap();
    Resource &backBufferRenderTarget = backBuffer.GetRenderTarget();

    HRESULT res = mDirectCommandAllocator->Reset();
    if (FAILED(res))
    {
        std::print("Failed to reset backBufferCommandAllocator");
    }
    
    res = mDirectCommandList->Reset(mDirectCommandAllocator.GetRaw(), nullptr);
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
    
        mDirectCommandList->ResourceBarrier(1, &barrier);
    
        constexpr std::array<float, 4> clearColour = { 0.6f, 0.9f, 0.5f, 1.0f };

        const CD3DX12_CPU_DESCRIPTOR_HANDLE rtv = CD3DX12_CPU_DESCRIPTOR_HANDLE(
            backBufferDescriptorHeap.GetCOM()->GetCPUDescriptorHandleForHeapStart(), 
            mSwapChainManager.GetCurrentBackBufferIndex(), 
            mSwapChainManager.GetRenderTargetDescriptorSize()
        );

        mDirectCommandList->ClearRenderTargetView(rtv, clearColour.data(), 0, nullptr);
        mDirectCommandList->OMSetRenderTargets(1, &rtv, false, nullptr);

        std::vector<ID3D12DescriptorHeap*> descriptorHeaps =
        {
            #if defined(_DEBUG)
            mImGuiDescriptorHeap.GetRaw()
            #endif
        };
        
        mDirectCommandList->SetDescriptorHeaps(static_cast<std::uint32_t>(descriptorHeaps.size()), descriptorHeaps.data());
        #if defined(_DEBUG)
        HexEngine::Graphics::UI::ImGuiTool::RenderDrawData(mDirectCommandList);
        #endif
    }

    // Present
    {
        CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
            backBufferRenderTarget.GetRaw(),
            D3D12_RESOURCE_STATE_RENDER_TARGET,  
            D3D12_RESOURCE_STATE_PRESENT
        );

        mDirectCommandList->ResourceBarrier(1, &barrier);

        DirectXUtils::ThrowIfFailed(mDirectCommandList->Close());

        // TODO: EXECUTE COMMAND LISTS :)
        
        //mDirectCommandQueue.AppendCommandList(mDirectCommandList);
        auto vec = std::vector{mDirectCommandList};
        mDirectCommandQueue.ExecuteCommandLists(vec);
    }
}