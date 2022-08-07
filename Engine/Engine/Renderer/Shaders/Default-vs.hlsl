#include "./Structures.hlsli"
#include "./Functions.hlsli"

VSOutput main(VSInput input)
{
    VSOutput output;
    
    Skinning(input.pos, input.boneWeights, input.boneIds);
    
    float4x4 viewProjection = transpose(mul(mul(projection, view), world));
    
    output.pos = mul(float4(input.pos.x, input.pos.y, input.pos.z, 1.f), viewProjection);
    output.texCoords = float2(input.texCoords.x, input.texCoords.y);
    return output;
};