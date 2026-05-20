#pragma once
#include "MeshData.hpp"

namespace HexEngine::Assets::MeshLoader
{
	constexpr D3D12_INPUT_ELEMENT_DESC c_DefualtElementDesc[3] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 1 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 1 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 1 }
	};

	MeshData LoadMesh(std::string_view filePath);

	void UploadMeshResources(
		winrt::com_ptr<ID3D12Device9> device,
		winrt::com_ptr<ID3D12CommandQueue> cmdQueue,
		winrt::com_ptr<ID3D12CommandAllocator> cmdAlloc,
		winrt::com_ptr<ID3D12GraphicsCommandList> cmdList,
		MeshData &meshData
	);
}
