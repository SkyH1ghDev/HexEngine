
uint NextRandom(inout uint state)
{
	state = state * 747796405u + 2891336453u;
	uint result = ((state >> ((state >> 28) + 4u)) ^ state) * 277803737u;
	result = (result >> 22) ^ result;
	return result;
}

float RandomValue(inout uint state)
{
	return float(NextRandom(state)) / 4294967295.0;
}

float3 UIDtoCol(uint id)
{
	return float3(
		RandomValue(id),
		RandomValue(id),
		RandomValue(id)
	);
}


struct PixelIn
{
	float4 position		: SV_Position;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	uint meshletIndex	: COLOR0;
};

struct PixelOut
{
	float4 color		: SV_TARGET;
};

PixelOut main(PixelIn input) : SV_TARGET
{
	PixelOut output;
	
	output.color = float4(UIDtoCol(input.meshletIndex), 1.0f);
	
	return output;
}
