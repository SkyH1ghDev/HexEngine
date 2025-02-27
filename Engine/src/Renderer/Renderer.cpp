#include "Renderer.hpp"
#include "RendererSetup.hpp"

Renderer::Renderer()
{
    // Only When _DEBUG Flag is set 
    this->m_debugInterface = RendererSetup::CreateDebugLayer();

    // Standard DirectX12 Initialization
    this->m_device = RendererSetup::CreateDevice();
}