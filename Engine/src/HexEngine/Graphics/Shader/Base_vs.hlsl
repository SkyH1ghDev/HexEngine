struct ModelViewProjection
{
    matrix MVP;
};

ConstantBuffer<ModelViewProjection> MatrixCB : register(b0);

struct VSInput 
{
    float4 Position : SV_POSITION;
    float4 Colour : COLOUR;
};

struct VSOutput
{
    float4 Position : SV_POSITION;
    float4 Colour   : COLOUR;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    output.Position = mul(MatrixCB.MVP, input.Position);
    output.Colour = input.Colour;

    return output;
}