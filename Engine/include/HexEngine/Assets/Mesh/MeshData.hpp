#pragma once
#include <DirectXCollision.h>
#include <winrt/base.h>

namespace HexEngine::Assets
{
	struct VertexData
	{
		float px{ 0 }, py{ 0 }, pz{ 0 };	// position
		float nx{ 0 }, ny{ 1 }, nz{ 0 };	// normal
		float u0{ 0 }, v0{ 0 };				// texcoords
	};

	struct MeshletData
	{
		std::uint32_t vertexCount{ 0 };
		std::uint32_t vertexOffset{ 0 };

		std::uint32_t primitiveCount{ 0 };
		std::uint32_t primitiveOffset{ 0 };
	};

	struct MeshletTriangleData
	{
		std::uint32_t i0 : 10 { 0 };
		std::uint32_t i1 : 10 { 0 };
		std::uint32_t i2 : 10 { 0 };
	};

	struct MeshletCullData
	{
		DirectX::BoundingSphere bounds{ { 0,0,0 }, 0 };
		DirectX::XMFLOAT3 coneAxis{ 0,1,0 };
		float coneCutoff{ 0 };
	};

	struct MeshInfo
	{
		std::uint32_t IndexSize{ 0 };
		std::uint32_t MeshletCount{ 0 };

		std::uint32_t LastMeshletVertCount{ 0 };
		std::uint32_t LastMeshletPrimCount{ 0 };
	};

	struct MeshResources
	{
		winrt::com_ptr<ID3D12Resource> vertexBuffer{ nullptr };
		winrt::com_ptr<ID3D12Resource> indexBuffer{ nullptr };

		winrt::com_ptr<ID3D12Resource> meshletBuffer{ nullptr };
		winrt::com_ptr<ID3D12Resource> meshletVerticesBuffer{ nullptr };	// unique vertex index
		winrt::com_ptr<ID3D12Resource> meshletTrianglesBuffer{ nullptr };	// primitives
		winrt::com_ptr<ID3D12Resource> meshletCullDataBuffer{ nullptr };

		winrt::com_ptr<ID3D12Resource> meshInfoBuffer{ nullptr };

		D3D12_VERTEX_BUFFER_VIEW vertexBufferView{ };
		D3D12_INDEX_BUFFER_VIEW  indexBufferView{ };
	};

	struct MeshData
	{
		std::vector<VertexData>		vertices{ };
		std::vector<std::uint16_t>	indices{ };

		std::vector<MeshletData>			meshlets{ };
		std::vector<std::uint32_t>			meshletVertices{ };		// unique vertex indices per meshlet
		std::vector<MeshletTriangleData>	meshletTriangles{ };	// primitive indices per meshlet
		std::vector<MeshletCullData>		meshletCullData{ };

		MeshResources resources{ };
	};
}

