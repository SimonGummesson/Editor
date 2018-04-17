TextureCube cubeMap : register(t0);
SamplerState Sampler : register(s0);

struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float3 Tex : TEXCOORD;
    float3 Normal : NORMAL;
};

float4 PS_main(VS_OUT input) : SV_Target
{
    return cubeMap.Sample(Sampler, input.Tex);
};