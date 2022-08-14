
void Skinning(inout float3 Pos, float4 weights, uint4 indicies)
{
    if (any(weights))
    {
        float3x4 m = 0;
        
        for (uint i = 0; i < 4; i++)
        {
            m += weights[i] * boneTransforms[indicies[i]];
        }

        Pos = mul(m, float4(Pos, 1));
    }
}