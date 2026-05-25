#pragma once
#include "MeshData.hpp"
#include <HexEngine/Graphics/DirectX12/Device.hpp>
#include <HexEngine/Graphics/DirectX12/CommandQueue.hpp>
#include <HexEngine/Graphics/DirectX12/CommandAllocator.hpp>
#include <HexEngine/Graphics/DirectX12/CommandList.hpp>

namespace HexEngine::Assets::MeshLoader
{
	constexpr D3D12_INPUT_ELEMENT_DESC cDefaultElementDesc[3] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	MeshData LoadMesh(std::string_view filePath);

	void UploadMeshResources(
		HexEngine::Graphics::DirectX12::Device &pDevice,
		HexEngine::Graphics::DirectX12::CommandQueue &pCmdQueue,
		HexEngine::Graphics::DirectX12::CommandAllocator &pCmdAlloc,
		HexEngine::Graphics::DirectX12::CommandList &pCmdList,
		MeshData &meshData
	);
}
