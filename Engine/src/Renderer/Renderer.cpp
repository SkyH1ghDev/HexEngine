#include "Renderer.hpp"

void Renderer::Setup()
{
    
}

void Renderer::EnableDebugLayer()
{
    // Enables Debug Layer
    #if defined(_DEBUG)
    MW::ComPtr<ID3D12Debug> debugInterface;
    #endif
}

