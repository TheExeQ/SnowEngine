#include "./Structures.hlsli"

cbuffer FrameBuffer : register(b0)
{
    float4x4 mat;
}

VSOutput main(VSInput input)
{
    VSOutput output;
    output.pos = mul(float4(input.pos.x, input.pos.y, input.pos.z, 1.f), mat);
    output.texCoords = float2(input.texCoords.x, input.texCoords.y);
    return output;
};