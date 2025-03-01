#include "Renderer.hpp"
#include "RendererSetup.hpp"
#include "ImGui/imgui.h"

Renderer::Renderer(const std::shared_ptr<SDL_Window>& window)
{
    // Only When _DEBUG Flag is set 
    m_debugInterface = RendererSetup::CreateDebugLayer();

    // Standard DirectX12 Initialization
    m_device = RendererSetup::CreateDevice();
    m_commandQueue = RendererSetup::CreateCommandQueue(m_device, D3D12_COMMAND_LIST_TYPE_DIRECT);
    m_swapChain = RendererSetup::CreateSwapChain(window, m_commandQueue, 1280, 720, 3);
    m_backBufferDescriptorHeap = RendererSetup::CreateDescriptorHeap(m_device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, g_NumFrames);
    m_backBuffers = RendererSetup::CreateBackBuffers(m_device, m_swapChain, m_backBufferDescriptorHeap, g_NumFrames);
    m_commandAllocator = RendererSetup::CreateCommandAllocator(m_device, D3D12_COMMAND_LIST_TYPE_DIRECT);
    m_commandList = RendererSetup::CreateCommandList(m_device, m_commandAllocator, D3D12_COMMAND_LIST_TYPE_DIRECT);
}


void Renderer::Render()
{
    
}