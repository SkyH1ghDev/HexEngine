#pragma once
#include <DirectXCollision.h>
#include <DirectXMesh.h>

namespace Assets
{
	struct VertexData
	{
		float px, py, pz;   // position
		float nx, ny, nz;   // normal
		float u0, v0;		// texcoords
	};

	struct MeshletData
	{
		std::uint32_t vertexCount;
		std::uint32_t vertexOffset;

		std::uint32_t primitiveCount;
		std::uint32_t primitiveOffset;

		DirectX::BoundingBox bounds;
		DirectX::XMFLOAT3 coneAxis;
		float coneCutoff;
	};

	struct MeshletTriangleData
	{
		uint32_t i0 : 10;
		uint32_t i1 : 10;
		uint32_t i2 : 10;
	};

	struct MeshData
	{
		std::vector<VertexData>		vertices;
		std::vector<std::size_t>	indices;

		// DirectXMesh discards submesh info (Huh???)

		std::vector<MeshletData>			meshlets;
		std::vector<std::uint32_t>			meshletVertices;	// unique vertex indices per meshlet
		std::vector<MeshletTriangleData>	meshletTriangles;
	};
}

