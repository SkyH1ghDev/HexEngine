#include "ShaderLoader.hpp"
#include <fstream>

using namespace winrt;
using namespace DirectX;
using namespace DirectXUtils;
using namespace HexEngine::Assets;
using namespace HexEngine::Graphics::DirectX12;

ShaderData HexEngine::Assets::ShaderLoader::LoadShader(std::string_view pPath)
{
	std::string key(pPath);

	// Load the shader data from the file
	std::ifstream file(key, std::ios::binary | std::ios::ate);
	if (!file)
	{
		throw std::runtime_error("Failed to open shader file: " + key);
	}

	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	auto buffer = std::make_unique<std::uint8_t[]>(size);
	if (!file.read(reinterpret_cast<char*>(buffer.get()), size))
	{
		throw std::runtime_error("Failed to read shader file: " + key);
	}

	ShaderData shaderData{std::move(buffer), static_cast<std::uint32_t>(size)};
	
	return shaderData;
}

RootSignatureData HexEngine::Assets::ShaderLoader::CreateRootSignature(Device &pDevice, const ShaderData &pShader)
{
	RootSignatureData rootSignatureData;

	// Create the root signature from the shader data
	ThrowIfFailed(pDevice->CreateRootSignature(
		0,
		pShader.data.get(),
		pShader.size,
		IID_PPV_ARGS(&rootSignatureData.rootSignature)
	));

	return rootSignatureData;
}

PipelineStateData HexEngine::Assets::ShaderLoader::CreateMeshPipelineState(
	Device &pDevice, 
	const RootSignatureData &pRootSignature, 
	const ShaderData *pAS, 
	const ShaderData *pMS, 
	const ShaderData *pPS)
{
	PipelineStateData pipelineStateData;

	D3DX12_MESH_SHADER_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.pRootSignature = pRootSignature.rootSignature.get();

	if (pAS) psoDesc.AS = { pAS->data.get(), pAS->size};
	if (pMS) psoDesc.MS = { pMS->data.get(), pMS->size };
	if (pPS) psoDesc.PS = { pPS->data.get(), pPS->size };

	// Assume default RTV format, disable depth
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.DSVFormat = DXGI_FORMAT_UNKNOWN;
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);		// CW front; cull back
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);					// Opaque
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState.DepthEnable = FALSE;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.SampleDesc = DefaultSampleDesc();

	auto psoStream = CD3DX12_PIPELINE_MESH_STATE_STREAM(psoDesc);

	D3D12_PIPELINE_STATE_STREAM_DESC streamDesc{};
	streamDesc.pPipelineStateSubobjectStream = &psoStream;
	streamDesc.SizeInBytes = sizeof(psoStream);

	ThrowIfFailed(pDevice->CreatePipelineState(&streamDesc, IID_PPV_ARGS(&pipelineStateData.pipelineState)));

	return pipelineStateData;
}

PipelineStateData HexEngine::Assets::ShaderLoader::CreateGraphicsPipelineState(
	Device &pDevice, 
	const RootSignatureData &pRootSignature,
	const D3D12_INPUT_ELEMENT_DESC *pInputLayout,
	std::uint16_t numInputElements,
	const ShaderData *pVS, 
	const ShaderData *pPS)
{
	PipelineStateData pipelineStateData;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.pRootSignature = pRootSignature.rootSignature.get();

	if (pVS) psoDesc.VS = { pVS->data.get(), pVS->size };
	if (pPS) psoDesc.PS = { pPS->data.get(), pPS->size };

	// Assume default RTV format, disable depth
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.DSVFormat = DXGI_FORMAT_UNKNOWN;
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);		// CW front; cull back
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);					// Opaque
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState.DepthEnable = FALSE;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.SampleDesc = DefaultSampleDesc();
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.InputLayout = { pInputLayout, numInputElements };
	
	ThrowIfFailed(pDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineStateData.pipelineState)));

	return pipelineStateData;
}
