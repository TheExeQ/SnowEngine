
void Skinning(inout float3 Pos, float4 weights, uint4 indicies)
{
    if (any(weights))
    {
        float3x3 m = 0;
        
        for (uint i = 0; i < 4; i++)
        {
            m += boneTransforms[indicies[i]] * weights[i];
        }

        Pos = mul(m, Pos);
    }
}