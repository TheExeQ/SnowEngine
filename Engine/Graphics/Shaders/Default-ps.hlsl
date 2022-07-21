#include "./Structures.hlsli"

PSOutput main(PSInput input)
{
    PSOutput output;
    output.color = float4(input.color.x, input.color.y, input.color.z, 1.0f);
    return output;
};