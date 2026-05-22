#include "MeshLoader.hpp"
#include <Utils/StringUtils.hpp>

#undef min
#undef max
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMesh.h>
#include <utils/WaveFrontReader.h>

using namespace winrt;
using namespace DirectX;
using namespace DirectXUtils;
using namespace HexEngine::Assets;
using namespace HexEngine::Graphics::DirectX12;

static void ComputeBoundingCone(
	XMVECTOR &outAxis, 
	float &outCutoff,
	size_t vertCount, 
	const std::unique_ptr<XMFLOAT3[]> &positions,
	size_t posStride, 
	const uint16_t *uniqueVertexIB)
{
	XMVECTOR coneAxis = XMVectorZero();
	float coneCutoff = 0.0f;

	for (size_t i = 0; i < vertCount; ++i)
	{
		XMVECTOR v = XMLoadFloat3(reinterpret_cast<const XMFLOAT3 *>(reinterpret_cast<const uint8_t *>(positions.get()) + uniqueVertexIB[i] * posStride));
		coneAxis = XMVectorAdd(coneAxis, v);
	}

	coneAxis = XMVector3Normalize(coneAxis);

	for (size_t i = 0; i < vertCount; ++i)
	{
		XMVECTOR v = XMLoadFloat3(reinterpret_cast<const XMFLOAT3 *>(reinterpret_cast<const uint8_t *>(positions.get()) + uniqueVertexIB[i] * posStride));
		float dot = XMVectorGetX(XMVector3Dot(v, coneAxis));

		if (dot > coneCutoff)
			coneCutoff = dot;
	}

	outAxis = coneAxis;
	outCutoff = coneCutoff;
}

static MeshletCullData ComputeMeshletBounds(
	const uint16_t *uniqueVertexIB, 
	size_t vertCount,
	const std::unique_ptr<XMFLOAT3[]> &positions)
{
	MeshletCullData cullData{};

	// Compute bounding sphere
	BoundingSphere::CreateFromPoints(cullData.bounds, vertCount, positions.get(), sizeof(XMFLOAT3));

	// Compute bounding cone
	XMVECTOR coneAxis;
	float coneCutoff;
	ComputeBoundingCone(coneAxis, coneCutoff, vertCount, positions, sizeof(XMFLOAT3), uniqueVertexIB);

	XMStoreFloat3(&cullData.coneAxis, coneAxis);
	cullData.coneCutoff = coneCutoff;

	return cullData;
}

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
	auto mesh = std::make_unique<DX::WaveFrontReader<uint16_t>>();

	HRESULT hr = mesh->Load(StringUtils::NarrowToWide(filePath).c_str());
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to load " + std::string(filePath) + " (" + StringUtils::HResultToString(hr) + ")");
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
		primitiveIndices
	);

	if (FAILED(hr))
	{
		throw std::runtime_error("ComputeMeshlets failed: " + StringUtils::HResultToString(hr));
	}

	const uint16_t* uniqueVertexIndices = reinterpret_cast<const uint16_t* >(uniqueVertexIB.data());

	// Store mesh data
	MeshData meshData;

	// Vertices
	meshData.vertices.resize(mesh->vertices.size());
	std::memcpy(meshData.vertices.data(), mesh->vertices.data(), sizeof(DX::WaveFrontReader<uint16_t>::Vertex) * mesh->vertices.size());

	// Indices
	meshData.indices.reserve(mesh->indices.size());
	for (const uint16_t &index : mesh->indices)
	{
		meshData.indices.emplace_back(index);
	}

	// Meshlets
	meshData.meshlets.reserve(meshlets.size());
	meshData.meshletCullData.reserve(meshlets.size());

	for (const Meshlet &m : meshlets)
	{
		MeshletData md{};
		md.vertexCount = m.VertCount;
		md.vertexOffset = m.VertOffset;
		md.primitiveCount = m.PrimCount;
		md.primitiveOffset = m.PrimOffset;
		meshData.meshlets.push_back(md);

		MeshletCullData mcd = ComputeMeshletBounds(&uniqueVertexIndices[m.VertOffset], m.VertCount, pos);
		meshData.meshletCullData.push_back(mcd);
	}

	return meshData;
}


template <typename T, typename U>
constexpr T DivRoundUp(T num, U denom)
{
	return (num + denom - 1) / denom;
}

void MeshLoader::UploadMeshResources(
	Device &pDevice,
	CommandQueue &pCmdQueue,
	CommandAllocator &pCmdAlloc,
	CommandList &pCmdList,
	MeshData &meshData)
{
	MeshResources &res = meshData.resources;

	// Create default resources
	auto vertexDesc				= CD3DX12_RESOURCE_DESC::Buffer(meshData.vertices.size() * sizeof(meshData.vertices[0]));
	auto indexDesc				= CD3DX12_RESOURCE_DESC::Buffer(meshData.indices.size() * sizeof(meshData.indices[0]));
	auto meshletDesc			= CD3DX12_RESOURCE_DESC::Buffer(meshData.meshlets.size() * sizeof(meshData.meshlets[0]));
	auto meshletVertexDesc		= CD3DX12_RESOURCE_DESC::Buffer(DivRoundUp(meshData.meshletVertices.size(), 4) * 4);
	auto meshletTriangleDesc	= CD3DX12_RESOURCE_DESC::Buffer(meshData.meshletTriangles.size() * sizeof(meshData.meshletTriangles[0]));
	auto meshletCullDataDesc	= CD3DX12_RESOURCE_DESC::Buffer(meshData.meshlets.size() * sizeof(MeshletCullData));
	auto meshInfoDesc			= CD3DX12_RESOURCE_DESC::Buffer(sizeof(MeshInfo));

	auto defaultHeap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	ThrowIfFailed(pDevice->CreateCommittedResource(&defaultHeap, D3D12_HEAP_FLAG_NONE, &vertexDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&res.vertexBuffer)));
	ThrowIfFailed(pDevice->CreateCommittedResource(&defaultHeap, D3D12_HEAP_FLAG_NONE, &indexDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&res.indexBuffer)));
	ThrowIfFailed(pDevice->CreateCommittedResource(&defaultHeap, D3D12_HEAP_FLAG_NONE, &meshletDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&res.meshletBuffer)));
	ThrowIfFailed(pDevice->CreateCommittedResource(&defaultHeap, D3D12_HEAP_FLAG_NONE, &meshletVertexDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&res.meshletVerticesBuffer)));
	ThrowIfFailed(pDevice->CreateCommittedResource(&defaultHeap, D3D12_HEAP_FLAG_NONE, &meshletTriangleDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&res.meshletTrianglesBuffer)));
	ThrowIfFailed(pDevice->CreateCommittedResource(&defaultHeap, D3D12_HEAP_FLAG_NONE, &meshletCullDataDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&res.meshletCullDataBuffer)));
	ThrowIfFailed(pDevice->CreateCommittedResource(&defaultHeap, D3D12_HEAP_FLAG_NONE, &meshInfoDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&res.meshInfoBuffer)));

	// Create views
	res.indexBufferView.BufferLocation = res.indexBuffer->GetGPUVirtualAddress();
	res.indexBufferView.Format = DXGI_FORMAT_R16_UINT;
	res.indexBufferView.SizeInBytes = static_cast<uint32_t>(meshData.indices.size() * sizeof(meshData.indices[0]));
	
	res.vertexBufferView.BufferLocation = res.vertexBuffer->GetGPUVirtualAddress();
	res.vertexBufferView.SizeInBytes = static_cast<uint32_t>(meshData.vertices.size() * sizeof(meshData.vertices[0]));
	res.vertexBufferView.StrideInBytes = sizeof(VertexData);

	// Create upload resources
	com_ptr<ID3D12Resource> vertexUpload;
	com_ptr<ID3D12Resource> indexUpload;
	com_ptr<ID3D12Resource> meshletUpload;
	com_ptr<ID3D12Resource> meshletVertexUpload;
	com_ptr<ID3D12Resource> meshletTriangleUpload;
	com_ptr<ID3D12Resource> meshletCullDataUpload;
	com_ptr<ID3D12Resource> meshInfoUpload;

	auto uploadHeap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	ThrowIfFailed(pDevice->CreateCommittedResource(&uploadHeap, D3D12_HEAP_FLAG_NONE, &vertexDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexUpload)));
	ThrowIfFailed(pDevice->CreateCommittedResource(&uploadHeap, D3D12_HEAP_FLAG_NONE, &indexDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&indexUpload)));
	ThrowIfFailed(pDevice->CreateCommittedResource(&uploadHeap, D3D12_HEAP_FLAG_NONE, &meshletDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&meshletUpload)));
	ThrowIfFailed(pDevice->CreateCommittedResource(&uploadHeap, D3D12_HEAP_FLAG_NONE, &meshletVertexDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&meshletVertexUpload)));
	ThrowIfFailed(pDevice->CreateCommittedResource(&uploadHeap, D3D12_HEAP_FLAG_NONE, &meshletTriangleDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&meshletTriangleUpload)));
	ThrowIfFailed(pDevice->CreateCommittedResource(&uploadHeap, D3D12_HEAP_FLAG_NONE, &meshletCullDataDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&meshletCullDataUpload)));
	ThrowIfFailed(pDevice->CreateCommittedResource(&uploadHeap, D3D12_HEAP_FLAG_NONE, &meshInfoDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&meshInfoUpload)));

	// Map & copy memory to upload heap
	{
		uint8_t *memory = nullptr;
		vertexUpload->Map(0, nullptr, reinterpret_cast<void **>(&memory));
		std::memcpy(memory, meshData.vertices.data(), meshData.vertices.size());
		vertexUpload->Unmap(0, nullptr);
	}

	{
		uint8_t *memory = nullptr;
		indexUpload->Map(0, nullptr, reinterpret_cast<void **>(&memory));
		std::memcpy(memory, meshData.indices.data(), meshData.indices.size());
		indexUpload->Unmap(0, nullptr);
	}

	{
		uint8_t *memory = nullptr;
		meshletUpload->Map(0, nullptr, reinterpret_cast<void **>(&memory));
		std::memcpy(memory, meshData.meshlets.data(), meshData.meshlets.size() * sizeof(meshData.meshlets[0]));
		meshletUpload->Unmap(0, nullptr);
	}

	{
		uint8_t *memory = nullptr;
		meshletVertexUpload->Map(0, nullptr, reinterpret_cast<void **>(&memory));
		std::memcpy(memory, meshData.meshletVertices.data(), meshData.meshletVertices.size() * sizeof(meshData.meshletVertices[0]));
		meshletVertexUpload->Unmap(0, nullptr);
	}

	{
		uint8_t *memory = nullptr;
		meshletTriangleUpload->Map(0, nullptr, reinterpret_cast<void **>(&memory));
		std::memcpy(memory, meshData.meshletTriangles.data(), meshData.meshletTriangles.size() * sizeof(meshData.meshletTriangles[0]));
		meshletTriangleUpload->Unmap(0, nullptr);
	}

	{
		uint8_t *memory = nullptr;
		meshletCullDataUpload->Map(0, nullptr, reinterpret_cast<void **>(&memory));
		std::memcpy(memory, meshData.meshletCullData.data(), meshData.meshletCullData.size() * sizeof(meshData.meshletCullData[0]));
		meshletCullDataUpload->Unmap(0, nullptr);
	}

	{
		MeshInfo info = {};
		info.IndexSize = sizeof(meshData.indices[0]);
		info.MeshletCount = static_cast<uint32_t>(meshData.meshlets.size());
		info.LastMeshletVertCount = meshData.meshlets.back().vertexCount;
		info.LastMeshletPrimCount = meshData.meshlets.back().primitiveCount;

		uint8_t *memory = nullptr;
		meshInfoUpload->Map(0, nullptr, reinterpret_cast<void **>(&memory));
		std::memcpy(memory, &info, sizeof(MeshInfo));
		meshInfoUpload->Unmap(0, nullptr);
	}

	// Populate our command list
	pCmdList->Reset(pCmdAlloc.GetRaw(), nullptr);

	D3D12_RESOURCE_BARRIER postCopyBarriers[7]{};

	pCmdList->CopyResource(res.vertexBuffer.get(), vertexUpload.get());
	postCopyBarriers[0] = CD3DX12_RESOURCE_BARRIER::Transition(res.vertexBuffer.get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	pCmdList->CopyResource(res.indexBuffer.get(), indexUpload.get());
	postCopyBarriers[1] = CD3DX12_RESOURCE_BARRIER::Transition(res.indexBuffer.get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	pCmdList->CopyResource(res.meshletBuffer.get(), meshletUpload.get());
	postCopyBarriers[2] = CD3DX12_RESOURCE_BARRIER::Transition(res.meshletBuffer.get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	pCmdList->CopyResource(res.meshletVerticesBuffer.get(), meshletVertexUpload.get());
	postCopyBarriers[3] = CD3DX12_RESOURCE_BARRIER::Transition(res.meshletVerticesBuffer.get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	pCmdList->CopyResource(res.meshletTrianglesBuffer.get(), meshletTriangleUpload.get());
	postCopyBarriers[4] = CD3DX12_RESOURCE_BARRIER::Transition(res.meshletTrianglesBuffer.get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	pCmdList->CopyResource(res.meshletCullDataBuffer.get(), meshletCullDataUpload.get());
	postCopyBarriers[5] = CD3DX12_RESOURCE_BARRIER::Transition(res.meshletCullDataBuffer.get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	pCmdList->CopyResource(res.meshInfoBuffer.get(), meshInfoUpload.get());
	postCopyBarriers[6] = CD3DX12_RESOURCE_BARRIER::Transition(res.meshInfoBuffer.get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

	pCmdList->ResourceBarrier(ARRAYSIZE(postCopyBarriers), postCopyBarriers);

	ThrowIfFailed(pCmdList->Close());

	ID3D12CommandList *ppCommandLists[] = { pCmdList.GetRaw()};
	pCmdQueue.GetCOM()->ExecuteCommandLists(1, ppCommandLists);
}
