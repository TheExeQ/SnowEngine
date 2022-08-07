
cbuffer FrameBuffer : register(b0)
{
    float4x4 view;
    float4x4 projection;
}

cbuffer ObjectBuffer : register(b1)
{
    float4x4 world;
    float4x4 boneTransforms[128];
}

struct VSInput
{
    float3 pos : POSITION;
    float2 texCoords : TEXCOORDS;
    uint4 boneIds : BONEIDS;
    float4 boneWeights : WEIGHTS;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    float2 texCoords : TEXCOORDS;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float2 texCoords : TEXCOORDS;
};

struct PSOutput
{
    float4 color : SV_TARGET;
};