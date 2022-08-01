#include "./Structures.hlsli"

cbuffer FrameBuffer : register(b0)
{
    float4x4 view;
    float4x4 projection;
}

cbuffer ObjectBuffer : register(b1)
{
    float4x4 world;
}

VSOutput main(VSInput input)
{
    VSOutput output;
    
    float4x4 viewProjection = transpose(mul(mul(projection, view), world));
    
    output.pos = mul(float4(input.pos.x, input.pos.y, input.pos.z, 1.f), viewProjection);
    output.texCoords = float2(input.texCoords.x, input.texCoords.y);
    return output;
};