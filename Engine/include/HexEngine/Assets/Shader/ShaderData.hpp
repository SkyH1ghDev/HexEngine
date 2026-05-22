#pragma once
#include <d3d12.h>
#include <winrt/base.h>

namespace HexEngine::Assets
{
    struct RootSignatureData
    {
        winrt::com_ptr<ID3D12RootSignature> rootSignature;
    };

    struct ShaderData
    {
        std::unique_ptr<std::uint8_t[]> data;
        std::uint32_t size;
    };

    struct PipelineStateData
    {
        winrt::com_ptr<ID3D12PipelineState> pipelineState;
    };
}

