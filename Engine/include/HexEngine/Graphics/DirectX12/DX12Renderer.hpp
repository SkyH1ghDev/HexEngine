#pragma once
#include <DirectXMath.h>
#include <HexEngine/SDL/SDLWindow.hpp>
#include <HexEngine/Graphics/DirectX12/Fence.hpp>
#include <HexEngine/Graphics/DirectX12/DebugLayer.hpp>
#include <HexEngine/Graphics/DirectX12/Device.hpp>
#include <HexEngine/Graphics/DirectX12/SwapChainManager.hpp>
#include <HexEngine/Graphics/DirectX12/CommandQueue.hpp>
#include <HexEngine/Graphics/DirectX12/CommandList.hpp>
#include <HexEngine/Assets/Mesh/MeshLoader.hpp>
#include <HexEngine/Assets/Shader/ShaderLoader.hpp>
#include <HexEngine/World/Camera.hpp>
#include <HexEngine/World/Object.hpp>

namespace HexEngine::Graphics::DirectX12
{
    class DX12Renderer {
    public:
        DX12Renderer() = default;
        DX12Renderer(const HexEngine::SDL::SDLWindow& pWindow);
        ~DX12Renderer();
        DX12Renderer(const DX12Renderer& other) = default;
        DX12Renderer& operator=(const DX12Renderer& other) = default;
        DX12Renderer(DX12Renderer&& other) noexcept = default;
        DX12Renderer& operator=(DX12Renderer&& other) noexcept = default;

		void Update();
        void Render();
        void Draw();

    private:
        std::uint8_t mNumFrames {3};
        bool mVSync {false};

        // ImGui
#if defined(USE_IMGUI)
        DebugLayer mDebugInterface;
        DescriptorHeap mImGuiDescriptorHeap;
#endif
    
        Device mDevice {};
        
        // COPY
        CommandQueue mCopyCommandQueue {};
        CommandAllocator mCopyCommandAllocator {};
        CommandList mCopyCommandList {};
        
        // DIRECT
        CommandQueue mDirectCommandQueue {};
        CommandAllocator mDirectCommandAllocator {};
        CommandList mDirectCommandList {};
        
        // COMPUTE
        CommandQueue mComputeCommandQueue {};
        CommandAllocator mComputeCommandAllocator {};
        CommandList mComputeCommandList {};
        
        SwapChainManager mSwapChainManager {};
        Fence mFence {};
        
		Assets::MeshData mMeshMaxwell {};
		Assets::MeshData mMeshWhiskers {};

		Assets::RootSignatureData mMeshRootSignature {};
		Assets::RootSignatureData mGraphicsRootSignature {};

		Assets::PipelineStateData mMeshPipelineState {};
		Assets::PipelineStateData mGraphicsPipelineState {};
        
		Assets::ShaderData mVShader {};
		Assets::ShaderData mMShader {};
		Assets::ShaderData mPShader {};

        D3D12_VIEWPORT mViewport {};
        D3D12_RECT mScissorRect {};

		World::Camera mCamera {};
		std::vector<World::Object> mObjects {};

		bool mUseMeshShader { true };

		bool mDoSpin { false };
		float mSpinMult { 1.0f };

        //winrt::com_ptr<ID3D12Device2> m_device;
        //MW::ComPtr<ID3D12CommandQueue> m_commandQueue;
        // MW::ComPtr<ID3D12DescriptorHeap> m_backBufferDescriptorHeap;
        // std::vector<MW::ComPtr<ID3D12Resource>> m_backBuffers;
        // std::vector<MW::ComPtr<ID3D12CommandAllocator>> m_backBufferCommandAllocators;
        //UINT m_backBufferIndex = 0;
    
        // Synchronization
        //MW::ComPtr<ID3D12Fence> m_fence;
        //std::uint64_t m_fenceValue = 0;
        //std::vector<std::uint64_t> m_frameFenceValues = { 0, 0, 0 };
        //HANDLE m_fenceEvent = nullptr;
    };
}
