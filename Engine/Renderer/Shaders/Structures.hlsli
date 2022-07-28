#pragma once

struct VSInput
{
    float3 pos : POSITION;
    float2 texCoords : TEXCOORDS;
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