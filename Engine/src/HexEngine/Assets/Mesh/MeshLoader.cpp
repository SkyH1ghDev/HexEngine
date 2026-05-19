#include "MeshLoader.hpp"
#include <Utils/StringUtils.hpp>

#undef min
#undef max
#include <DirectXMesh.h>
#include <utils/WaveFrontReader.h>

using namespace Assets;
using namespace DirectX;

static void ComputeBoundingCone(
	XMVECTOR &outAxis, float &outCutoff,
	size_t vertCount, const XMFLOAT3 *positions, size_t posStride, const uint8_t *uniqueVertexIB)
{
	// Simple approach: Use the average normal as the cone axis and compute the cutoff based on the max angle to any vertex
	XMVECTOR avgNormal = XMVectorZero();
	for (size_t i = 0; i < vertCount; ++i)
	{
		XMVECTOR pos = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(reinterpret_cast<const uint8_t*>(positions) + uniqueVertexIB[i] * posStride));
		avgNormal = XMVectorAdd(avgNormal, pos);
	}

	avgNormal = XMVector3Normalize(avgNormal);
	outAxis = avgNormal;
	float maxAngleCos = 1.0f; // Cosine of the angle between the axis and any vertex

	for (size_t i = 0; i < vertCount; ++i)
	{
		XMVECTOR pos = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(reinterpret_cast<const uint8_t*>(positions) + uniqueVertexIB[i] * posStride));
		XMVECTOR dir = XMVector3Normalize(pos);
		float angleCos = XMVectorGetX(XMVector3Dot(avgNormal, dir));
		maxAngleCos = std::min(maxAngleCos, angleCos);
	}

	outCutoff = maxAngleCos;
}

static void ComputeMeshletBounds(
	const uint8_t *uniqueVertexIB, size_t vertCount,
	const XMFLOAT3 *positions,
	BoundingBox &outBounds, XMFLOAT3 &outConeAxis, float &outConeCutoff)
{
	// Compute bounding sphere
	BoundingSphere meshletSphere;
	BoundingSphere::CreateFromPoints(meshletSphere, vertCount, positions, sizeof(XMFLOAT3));

	// Compute bounding cone
	XMVECTOR coneAxis;
	float coneCutoff;
	ComputeBoundingCone(coneAxis, coneCutoff, vertCount, positions, sizeof(XMFLOAT3), uniqueVertexIB);

	XMStoreFloat3(&outConeAxis, coneAxis);
	outConeCutoff = coneCutoff;

	// Convert sphere to box
	BoundingBox::CreateFromSphere(outBounds, meshletSphere);
}

static void GenMeshletData(MeshData &meshData)
{
	// 1. Build position buffer
	std::vector<XMFLOAT3> positions;
	positions.resize(meshData.vertices.size());

	for (size_t i = 0; i < meshData.vertices.size(); ++i)
	{
		positions[i] = {
			meshData.vertices[i].px,
			meshData.vertices[i].py,
			meshData.vertices[i].pz
		};
	}

	// 2. Convert indices to uint32_t (DirectXMesh expects 32-bit indices)
	std::vector<uint32_t> indices32;
	indices32.reserve(meshData.indices.size());

	for (auto idx : meshData.indices)
		indices32.push_back(static_cast<uint32_t>(idx));

	// 3. Meshlet outputs
	std::vector<Meshlet> meshlets;
	std::vector<uint8_t> uniqueVertexIB;
	std::vector<MeshletTriangle> primitiveIB;

	HRESULT hr = ComputeMeshlets(
		indices32.data(),
		static_cast<std::size_t>(indices32.size() / 3),
		positions.data(),
		positions.size(),
		nullptr, // No adjacency
		meshlets,
		uniqueVertexIB,
		primitiveIB
	);

	if (FAILED(hr))
		throw std::runtime_error("ComputeMeshlets failed");

	// 5. Store meshlets
	meshData.meshlets.clear();
	meshData.meshlets.reserve(meshlets.size());

	for (const auto &m : meshlets)
	{
		MeshletData md{};

		md.vertexCount = m.VertCount;
		md.vertexOffset = m.VertOffset;

		md.primitiveCount = m.PrimCount;
		md.primitiveOffset = m.PrimOffset;

		// Compute bounds + cone
		ComputeMeshletBounds(
			&uniqueVertexIB[m.VertOffset],
			m.VertCount,
			positions.data(),
			md.bounds,
			md.coneAxis,
			md.coneCutoff
		);

		meshData.meshlets.push_back(md);
	}

	// 6. Store meshlet vertex + primitive streams
	//meshData.meshletVertices = std::move(uniqueVertexIB);

	meshData.meshletTriangles.resize(primitiveIB.size());
	std::memcpy(
		meshData.meshletTriangles.data(),
		primitiveIB.data(),
		primitiveIB.size() * sizeof(MeshletTriangle)
	);
}

static MeshData GenMeshData(DX::WaveFrontReader<uint16_t>* mesh)
{
	MeshData meshData;

	meshData.vertices.resize(mesh->vertices.size());
	std::memcpy(meshData.vertices.data(), mesh->vertices.data(), sizeof(DX::WaveFrontReader<uint16_t>::Vertex) * mesh->vertices.size());

	meshData.indices.reserve(mesh->indices.size());
	for (const auto &index : mesh->indices)
	{
		meshData.indices.emplace_back(index);
	}

	GenMeshletData(meshData);

	return meshData;
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

	auto mesh = std::make_unique<DX::WaveFrontReader<uint16_t>>();

	HRESULT hr = mesh->Load(StringUtils::NarrowToWide(filePath).c_str());
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to load " + std::string(filePath) + " (" + StringUtils::HResultToString(hr) + ")");
	}

	return GenMeshData(mesh.get());
}
