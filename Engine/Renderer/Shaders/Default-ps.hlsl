#include "./Structures.hlsli"

Texture2D objectTexture : TEXTURE : register(t0);
SamplerState objectSamplerState : SAMPLER : register(s0);

PSOutput main(PSInput input)
{
    PSOutput output;
    float3 pixelColor = objectTexture.Sample(objectSamplerState, input.texCoords).rgb;
    output.color = float4(pixelColor.x, pixelColor.y, pixelColor.z, 1.0f);
    return output;
};