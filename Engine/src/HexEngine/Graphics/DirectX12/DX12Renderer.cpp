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
    
    
    
    
    
    
    // CREATE VERTEX BUFFER
    CD3DX12_HEAP_PROPERTIES vertexBufferHeapProps = CD3DX12_HEAP_PROPERTIES {D3D12_HEAP_TYPE_DEFAULT};
    
    CD3DX12_RESOURCE_DESC vertexBufferResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(_countof(g_Vertices) * sizeof(VertexPosColor), D3D12_RESOURCE_FLAG_NONE); 
    
    winrt::com_ptr<ID3D12Resource> vertexBuffer {nullptr};
    DirectXUtils::ThrowIfFailed(
        mDevice->CreateCommittedResource(
            &vertexBufferHeapProps, D3D12_HEAP_FLAG_NONE,
            &vertexBufferResourceDesc, D3D12_RESOURCE_STATE_COMMON, nullptr,
            IID_PPV_ARGS(&vertexBuffer)
        )
    );
    
    CD3DX12_HEAP_PROPERTIES vertexIntermediaryBufferHeapProps = CD3DX12_HEAP_PROPERTIES{D3D12_HEAP_TYPE_UPLOAD};
    
    CD3DX12_RESOURCE_DESC vertexIntermediaryBufferResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(_countof(g_Vertices) * sizeof(VertexPosColor));
    
    winrt::com_ptr<ID3D12Resource> vertexIntermediaryBuffer {nullptr};
    DirectXUtils::ThrowIfFailed(
        mDevice->CreateCommittedResource(
            &vertexIntermediaryBufferHeapProps, D3D12_HEAP_FLAG_NONE,
            &vertexIntermediaryBufferResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
            IID_PPV_ARGS(&vertexIntermediaryBuffer)
        )    
    );
    
    D3D12_SUBRESOURCE_DATA vertexSubresourceData 
    {
        .pData = g_Vertices,
        .RowPitch = _countof(g_Vertices) * sizeof(VertexPosColor),
        .SlicePitch = _countof(g_Vertices) * sizeof(VertexPosColor),
    };
    
    CD3DX12_RESOURCE_BARRIER vertexBufferBarrier = CD3DX12_RESOURCE_BARRIER::Transition(vertexBuffer.get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);
    mCopyCommandList->ResourceBarrier(1, &vertexBufferBarrier);
    
    UpdateSubresources(mCopyCommandList.GetRaw(), vertexBuffer.get(), vertexIntermediaryBuffer.get(), 0, 0, 1, &vertexSubresourceData);
    
    
    
    // CREATE INDEX BUFFER
    CD3DX12_HEAP_PROPERTIES indexBufferHeapProps = CD3DX12_HEAP_PROPERTIES {D3D12_HEAP_TYPE_DEFAULT};
    
    CD3DX12_RESOURCE_DESC indexBufferResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(_countof(g_Vertices) * 2, D3D12_RESOURCE_FLAG_NONE); 
    
    winrt::com_ptr<ID3D12Resource> indexBuffer {nullptr};
    DirectXUtils::ThrowIfFailed(
        mDevice->CreateCommittedResource(
            &indexBufferHeapProps, D3D12_HEAP_FLAG_NONE,
            &indexBufferResourceDesc, D3D12_RESOURCE_STATE_COMMON, nullptr,
            IID_PPV_ARGS(&indexBuffer)
        )
    );
    
    CD3DX12_HEAP_PROPERTIES indexIntermediaryBufferHeapProps = CD3DX12_HEAP_PROPERTIES{D3D12_HEAP_TYPE_UPLOAD};
    
    CD3DX12_RESOURCE_DESC indexIntermediaryBufferResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(_countof(g_Vertices) * 2);
    
    winrt::com_ptr<ID3D12Resource> indexIntermediaryBuffer {nullptr};
    DirectXUtils::ThrowIfFailed(
        mDevice->CreateCommittedResource(
            &indexIntermediaryBufferHeapProps, D3D12_HEAP_FLAG_NONE,
            &indexIntermediaryBufferResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
            IID_PPV_ARGS(&indexIntermediaryBuffer)
        )    
    );
    
    D3D12_SUBRESOURCE_DATA indexSubresourceData 
    {
        .pData = g_Indices,
        .RowPitch = _countof(g_Vertices) * 2,
        .SlicePitch = _countof(g_Vertices) * 2
    };
    
    CD3DX12_RESOURCE_BARRIER indexBufferBarrier = CD3DX12_RESOURCE_BARRIER::Transition(indexBuffer.get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);
    mCopyCommandList->ResourceBarrier(1, &indexBufferBarrier);
    
    UpdateSubresources(mCopyCommandList.GetRaw(), indexBuffer.get(), indexIntermediaryBuffer.get(), 0, 0, 1, &indexSubresourceData);
    
    DirectXUtils::ThrowIfFailed(mCopyCommandList->Close());
    
    mCopyCommandQueue->ExecuteCommandLists(1, std::vector<ID3D12CommandList*>({mCopyCommandList.GetRaw()}).data());
    
    D3D12_INPUT_ELEMENT_DESC inputLayout[]
    {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 1 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 1 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 1 }
    };
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
 
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    /*
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
    }*/
}