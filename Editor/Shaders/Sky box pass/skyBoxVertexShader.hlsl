cbuffer WVPMatrix : register(b0)
{
    matrix WVPMatrix;
    matrix WorldMatrix;
};

struct VS_IN
{
    float3 Pos : POSITION;
    float2 Tex : TEXCOORD;
    float3 Normal : NORMAL;
};

struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float3 Tex : TEXCOORD;
    float3 Normal : NORMAL;
};
//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
    VS_OUT output = (VS_OUT)0;

    output.Pos = mul(float4(input.Pos, 1.f), WVPMatrix).xyww;
    output.Tex = input.Pos;
    output.Normal = mul(float4(input.Normal, 0.f), WorldMatrix).xyz;

    return output;
}