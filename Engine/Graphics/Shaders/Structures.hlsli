#pragma once

struct VSInput
{
    float3 pos : POSITION;
    float3 color : COLOR;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
};

struct PSOutput
{
    float4 color : SV_TARGET;
};