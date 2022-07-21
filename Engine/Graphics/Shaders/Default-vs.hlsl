#include "./Structures.hlsli"

cbuffer Constants : register(b0)
{
    float4x4 mat;
}

VSOutput main(VSInput input)
{
    VSOutput output;
    output.pos = mul(float4(input.pos.x, input.pos.y, input.pos.z, 1.f), mat);
    output.color = float3(input.color.x, input.color.y, input.color.z);
    return output;
};