StructuredBuffer<float> buffer0;
RWStructuredBuffer<float> result;

[shader("compute")]
[numthreads(1,1,1)]
void ComputeMain(uint3 tID : SV_DispatchThreadID)
{
    uint idx = tID.x;
    result[idx] = buffer0[idx];
}