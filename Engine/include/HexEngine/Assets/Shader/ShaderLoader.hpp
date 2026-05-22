#pragma once
#include "ShaderData.hpp"
#include <HexEngine/Graphics/DirectX12/Device.hpp>

namespace HexEngine::Assets::ShaderLoader
{
	ShaderData LoadShader(std::string_view pPath);

	RootSignatureData CreateRootSignature(
		HexEngine::Graphics::DirectX12::Device &pDevice, 
		const ShaderData &pShader
	);

	PipelineStateData CreateMeshPipelineState(
		HexEngine::Graphics::DirectX12::Device &pDevice, 
		const RootSignatureData &pRootSignature,
		const ShaderData *pAS = nullptr, 
		const ShaderData *pMS = nullptr, 
		const ShaderData *pPS = nullptr
	);

	PipelineStateData CreateGraphicsPipelineState(
		HexEngine::Graphics::DirectX12::Device &pDevice, 
		const RootSignatureData &pRootSignature,
		const D3D12_INPUT_ELEMENT_DESC *pInputLayout, // NOTE: Provided in MeshLoader.hpp
		std::uint16_t numInputElements,
		const ShaderData *pVS = nullptr,
		const ShaderData *pPS = nullptr
	);
}
