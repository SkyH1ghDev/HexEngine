#include <PCH/EnginePCH.hpp>
#include "MeshLoader.hpp"
#include <Utils/StringUtils.hpp>

#undef min
#undef max
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMesh.h>
#include <utils/WaveFrontReader.h>

#define MAX_MESHLET_VERTS 64
#define MAX_MESHLET_PRIMS 126

using namespace winrt;
using namespace DirectX;
using namespace DirectXUtils;
using namespace HexEngine::Assets;
using namespace HexEngine::Graphics::DirectX12;

MeshData MeshLoader::LoadMesh(std::string_view filePath)
{
	// Ensure the file exists and is a .obj format
	if (filePath.substr(filePath.find_last_of('.')) != ".obj")
	{
		throw std::runtime_error("Error: Unsupported file format: " + std::string(filePath));
	}

	if (!std::filesystem::exists(filePath))
	{
		throw std::runtime_error("Error: File does not exist: " + std::string(filePath));
	}

	// Load Wavefront OBJ
	auto mesh = std::make_unique<DX::WaveFrontReader<std::uint32_t>>();

	HRESULT hr = mesh->Load(StringUtils::NarrowToWide(filePath).c_str());
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to load mesh: " + std::string(filePath) + " (" + StringUtils::HResultToString(hr) + ")");
	}

	size_t nFaces = mesh->indices.size() / 3;
	size_t nVerts = mesh->vertices.size();

	// Generate meshlets
	auto pos = std::make_unique<XMFLOAT3[]>(nVerts);
	for (size_t j = 0; j < nVerts; ++j)
		pos[j] = mesh->vertices[j].position;

	std::vector<Meshlet> meshlets;
	std::vector<uint8_t> uniqueVertexIB;
	std::vector<MeshletTriangle> primitiveIndices;

	hr = ComputeMeshlets(
		mesh->indices.data(), 
		nFaces,
		pos.get(), 
		nVerts,
		nullptr,
		meshlets, 
		uniqueVertexIB, 
		primitiveIndices,
		MAX_MESHLET_VERTS,
		MAX_MESHLET_PRIMS
	);

	if (FAILED(hr))
	{
		throw std::runtime_error("ComputeMeshlets failed: " + StringUtils::HResultToString(hr));
	}

	std::vector<std::uint32_t> uniqueVertexIB32;
	uniqueVertexIB32.reserve(uniqueVertexIB.size() / sizeof(std::uint32_t));
	for (int i = 0; i < uniqueVertexIB.size(); i += sizeof(std::uint32_t))
	{
		std::uint32_t index = *reinterpret_cast<const std::uint32_t *>(uniqueVertexIB.data() + i);
		uniqueVertexIB32.emplace_back(index);
	}

	// Store mesh data
	MeshData meshData;

	// Vertices
	meshData.vertices.resize(mesh->vertices.size());
	std::memcpy(meshData.vertices.data(), mesh->vertices.data(), sizeof(DX::WaveFrontReader<std::uint32_t>::Vertex) * mesh->vertices.size());

	// Indices
	meshData.indices.reserve(mesh->indices.size());
	for (const std::uint32_t &index : mesh->indices)
	{
		meshData.indices.emplace_back(index);
	}

	// Meshlets
	meshData.meshlets.reserve(meshlets.size());

	for (const Meshlet &m : meshlets)
	{
		MeshletData md{};
		md.vertexCount = m.VertCount;
		md.vertexOffset = m.VertOffset;
		md.primitiveCount = m.PrimCount;
		md.primitiveOffset = m.PrimOffset;
		meshData.meshlets.push_back(md);
	}

	meshData.meshletVertices.reserve(uniqueVertexIB32.size());
	for (const std::uint32_t &index : uniqueVertexIB32)
	{
		meshData.meshletVertices.push_back(index);
	}

	meshData.meshletTriangles.reserve(primitiveIndices.size());
	for (const MeshletTriangle &tri : primitiveIndices)
	{
		MeshletTriangleData mtd{};
		mtd.i0 = tri.i0;
		mtd.i1 = tri.i1;
		mtd.i2 = tri.i2;
		meshData.meshletTriangles.push_back(mtd);
	}

	return meshData;
}


void MeshLoader::UploadMeshResources(
	Device &pDevice,
	CommandQueue &pCmdQueue,
	CommandAllocator &pCmdAlloc,
	CommandList &pCmdList,
	MeshData &meshData)
{
	MeshResources &res = meshData.resources;

	// Create DEFAULT resources
	auto vertexDesc = CD3DX12_RESOURCE_DESC::Buffer(meshData.vertices.size() * sizeof(VertexData));
	auto indexDesc = CD3DX12_RESOURCE_DESC::Buffer(meshData.indices.size() * sizeof(meshData.indices[0]));
	auto meshletDesc = CD3DX12_RESOURCE_DESC::Buffer(meshData.meshlets.size() * sizeof(meshData.meshlets[0]));
	auto meshletVertexDesc = CD3DX12_RESOURCE_DESC::Buffer(meshData.meshletVertices.size() * sizeof(meshData.meshletVertices[0]));
	auto meshletTriangleDesc = CD3DX12_RESOURCE_DESC::Buffer(meshData.meshletTriangles.size() * sizeof(meshData.meshletTriangles[0]));

	auto defaultHeap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	ThrowIfFailed(pDevice->CreateCommittedResource(&defaultHeap, D3D12_HEAP_FLAG_NONE, &vertexDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&res.vertexBuffer)));
	ThrowIfFailed(pDevice->CreateCommittedResource(&defaultHeap, D3D12_HEAP_FLAG_NONE, &indexDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&res.indexBuffer)));
	ThrowIfFailed(pDevice->CreateCommittedResource(&defaultHeap, D3D12_HEAP_FLAG_NONE, &meshletDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&res.meshletBuffer)));
	ThrowIfFailed(pDevice->CreateCommittedResource(&defaultHeap, D3D12_HEAP_FLAG_NONE, &meshletVertexDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&res.meshletVerticesBuffer)));
	ThrowIfFailed(pDevice->CreateCommittedResource(&defaultHeap, D3D12_HEAP_FLAG_NONE, &meshletTriangleDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&res.meshletTrianglesBuffer)));

	// Create views
	res.indexBufferView.BufferLocation = res.indexBuffer->GetGPUVirtualAddress();
	res.indexBufferView.Format = (sizeof(meshData.indices[0]) == 2) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	res.indexBufferView.SizeInBytes = static_cast<std::uint32_t>(meshData.indices.size() * sizeof(meshData.indices[0]));
	res.vertexBufferView.BufferLocation = res.vertexBuffer->GetGPUVirtualAddress();
	res.vertexBufferView.SizeInBytes = static_cast<std::uint32_t>(meshData.vertices.size() * sizeof(VertexData));
	res.vertexBufferView.StrideInBytes = sizeof(VertexData);

	// Upload buffers (UPLOAD heap)
	com_ptr<ID3D12Resource> vertexUpload;
	com_ptr<ID3D12Resource> indexUpload;
	com_ptr<ID3D12Resource> meshletUpload;
	com_ptr<ID3D12Resource> meshletVertexUpload;
	com_ptr<ID3D12Resource> meshletTriangleUpload;
	com_ptr<ID3D12Resource> meshletCullDataUpload;

	auto uploadHeap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	ThrowIfFailed(pDevice->CreateCommittedResource(&uploadHeap, D3D12_HEAP_FLAG_NONE, &vertexDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexUpload)));
	ThrowIfFailed(pDevice->CreateCommittedResource(&uploadHeap, D3D12_HEAP_FLAG_NONE, &indexDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&indexUpload)));
	ThrowIfFailed(pDevice->CreateCommittedResource(&uploadHeap, D3D12_HEAP_FLAG_NONE, &meshletDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&meshletUpload)));
	ThrowIfFailed(pDevice->CreateCommittedResource(&uploadHeap, D3D12_HEAP_FLAG_NONE, &meshletVertexDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&meshletVertexUpload)));
	ThrowIfFailed(pDevice->CreateCommittedResource(&uploadHeap, D3D12_HEAP_FLAG_NONE, &meshletTriangleDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&meshletTriangleUpload)));

	// Copy CPU -> upload heap
	auto CopyToUpload = [](ID3D12Resource *res, auto *data, std::size_t sizeBytes) {
		std::uint8_t *mem = nullptr;
		res->Map(0, nullptr, reinterpret_cast<void **>(&mem));
		std::memcpy(mem, data, sizeBytes);
		res->Unmap(0, nullptr);
	};

	CopyToUpload(vertexUpload.get(), meshData.vertices.data(), meshData.vertices.size() * sizeof(meshData.vertices[0]));
	CopyToUpload(indexUpload.get(), meshData.indices.data(), meshData.indices.size() * sizeof(meshData.indices[0]));
	CopyToUpload(meshletUpload.get(), meshData.meshlets.data(),	meshData.meshlets.size() * sizeof(meshData.meshlets[0]));
	CopyToUpload(meshletVertexUpload.get(), meshData.meshletVertices.data(), meshData.meshletVertices.size() * sizeof(meshData.meshletVertices[0]));
	CopyToUpload(meshletTriangleUpload.get(), meshData.meshletTriangles.data(),	meshData.meshletTriangles.size() * sizeof(meshData.meshletTriangles[0]));

	// GPU upload pass
	pCmdList->Reset(pCmdAlloc.GetRaw(), nullptr);

	std::vector<D3D12_RESOURCE_BARRIER> barriers;

	auto AddBufferUpload = [&](ID3D12Resource *dst, ID3D12Resource *src, D3D12_RESOURCE_STATES finalState) {
		barriers.push_back(CD3DX12_RESOURCE_BARRIER::Transition(dst, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
		pCmdList->ResourceBarrier(1, &barriers.back());

		pCmdList->CopyResource(dst, src);
		barriers.push_back(CD3DX12_RESOURCE_BARRIER::Transition(dst, D3D12_RESOURCE_STATE_COPY_DEST, finalState));
		pCmdList->ResourceBarrier(1, &barriers.back());
	};

	AddBufferUpload(res.vertexBuffer.get(), vertexUpload.get(), D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
	AddBufferUpload(res.indexBuffer.get(), indexUpload.get(), D3D12_RESOURCE_STATE_INDEX_BUFFER);
	AddBufferUpload(res.meshletBuffer.get(), meshletUpload.get(), D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
	AddBufferUpload(res.meshletVerticesBuffer.get(), meshletVertexUpload.get(), D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
	AddBufferUpload(res.meshletTrianglesBuffer.get(), meshletTriangleUpload.get(), D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);

	ThrowIfFailed(pCmdList->Close());

	ID3D12CommandList *lists[] = { pCmdList.GetRaw() };
	pCmdQueue.GetCOM()->ExecuteCommandLists(1, lists);

	// Fence sync
	com_ptr<ID3D12Fence> fence;
	ThrowIfFailed(pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));

	pCmdQueue.GetCOM()->Signal(fence.get(), 1);

	if (fence->GetCompletedValue() != 1)
	{
		HANDLE event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		fence->SetEventOnCompletion(1, event);

		WaitForSingleObjectEx(event, 10000, false);
		CloseHandle(event);
	}
}