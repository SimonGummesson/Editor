cbuffer WVPMatrix : register(b0)
{
    matrix WVPMatrix;
    matrix WorldMatrix;
};

struct VS_IN
{
	float3 Pos : POSITION;
	float3 Color : COLOR;
    float3 Nor : NORMAL;
};

struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float3 WPos : POSITION;
    float3 Color : COLOR;
    float3 Normal : NORMAL;
};
//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

    output.Pos = mul(float4(input.Pos, 1.f), WVPMatrix);
    output.WPos = mul(float4(input.Pos, 1.f), WorldMatrix).xyz;
    output.Normal = mul(float4(input.Nor, 0.f), WorldMatrix).xyz;
	output.Color = input.Color;

	return output;
}