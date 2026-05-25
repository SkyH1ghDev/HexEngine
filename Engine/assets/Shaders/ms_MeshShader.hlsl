

// ================================= Defines ==================================

#define MAX_MESHLET_VERTS 64
#define MAX_MESHLET_PRIMS 126

// ============================================================================



// ================================ Structures ================================

struct Constants
{
	float4x4 World;
	float4x4 WorldView;
	float4x4 WorldViewProj;
};

struct MeshInfo
{
	uint indexBytes;
	uint meshletOffset;
};

struct Vertex
{
	float3 position;
	float3 normal;
	float2 uv;
};

struct Meshlet
{
	uint vertCount;
	uint vertOffset;
	uint triCount;
	uint triOffset;
};

struct VertexOut
{
	float4 position		: SV_Position;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	uint meshletIndex	: COLOR0;
};

// ============================================================================



// ================================ Resources =================================

ConstantBuffer<Constants>	bConstants				: register(b0);
ConstantBuffer<MeshInfo>	bMeshInfo				: register(b1);

StructuredBuffer<Vertex>	tVertices				: register(t0);
StructuredBuffer<Meshlet>	tMeshlets				: register(t1);
ByteAddressBuffer			tUniqueVertexIndices	: register(t2);
StructuredBuffer<uint>		tPrimitiveIndices		: register(t3);

// ============================================================================



// ================================= Helpers ==================================

uint3 UnpackTriangle(uint tri)
{
    // Unpacks a 10 bits per index triangle from a 32-bit uint.
	return uint3(tri & 0x3FF, (tri >> 10) & 0x3FF, (tri >> 20) & 0x3FF);
}

uint LoadUniqueVertexIndex(uint meshletVertexIndex)
{
	if (bMeshInfo.indexBytes == 4)
	{
		return tUniqueVertexIndices.Load(meshletVertexIndex * 4);
	}
	else
	{
        // packed uint16 indices
		uint packed = tUniqueVertexIndices.Load((meshletVertexIndex / 2) * 4);

		if ((meshletVertexIndex & 1) == 0)
			return packed & 0xFFFF;
		else
			return (packed >> 16) & 0xFFFF;
	}
}

// ============================================================================



// =================================== Main ===================================

[RootSignature("CBV(b0), RootConstants(b1, num32bitconstants=2), SRV(t0), SRV(t1), SRV(t2), SRV(t3)")]
[NumThreads(128, 1, 1)]
[OutputTopology("triangle")]
void main(
    uint gtid : SV_GroupThreadID,
    uint gid : SV_GroupID,
    out vertices VertexOut verts[MAX_MESHLET_VERTS],
    out indices uint3 tris[MAX_MESHLET_PRIMS]
)
{
	// Select meshlet
	Meshlet meshlet = tMeshlets[bMeshInfo.meshletOffset + gid];

	SetMeshOutputCounts(meshlet.vertCount, meshlet.triCount);
	
    // Emit vertices
	if (gtid < meshlet.vertCount)
	{
		uint meshletVertexIndex = meshlet.vertOffset + gtid;

        // Resolve vertex index
		uint vertexIndex = LoadUniqueVertexIndex(meshletVertexIndex);

		Vertex v = tVertices[vertexIndex];

		VertexOut outVert;
		//outVert.position = mul(float4(v.position, 1.0f), bConstants.WorldViewProj);
		//outVert.normal = mul(float4(v.normal, 0.0f), bConstants.WorldView).xyz;
		outVert.position = mul(bConstants.WorldViewProj, float4(v.position, 1.0f));
		outVert.normal = mul(bConstants.WorldViewProj, float4(v.normal, 0.0f)).xyz;
		outVert.uv = v.uv;
		outVert.meshletIndex = gid;

		verts[gtid] = outVert;
	}
	
    // Emit primitive indices
	if (gtid < meshlet.triCount)
	{
		uint packedTri = tPrimitiveIndices[meshlet.triOffset + gtid];
		
		uint3 tri = UnpackTriangle(packedTri);

		tris[gtid] = tri;
	}
}

// ============================================================================
