struct VertexIn
{
    float3 Position		: POSITION;
    float3 Normal		: NORMAL;
    float2 UV			: TEXCOORD;
};

struct VertexOut 
{
    float4 Position     : POSITION;
    float3 Normal       : NORMAL;
    float2 UV           : TEXCOORD;
    uint lmao           : COLOR0;
};

struct Constants
{
    float4x4 World;
    float4x4 WorldView;
    float4x4 WorldViewProj;
};

ConstantBuffer<Constants> bConstants    : register(b0);

[RootSignature("CBV(b0)")]
VertexOut main(VertexIn pInput)
{
    VertexOut output;
    
    output.Position = mul(bConstants.WorldViewProj, pInput.Position);;
    output.Normal = pInput.Normal;
    output.UV = pInput.UV;
    output.lmao = 0;
    
    return output;
}