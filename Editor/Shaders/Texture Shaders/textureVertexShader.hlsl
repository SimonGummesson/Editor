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
	float2 Tex : TEXCOORD;
	float3 Normal : NORMAL;
	float3 WPos : POSITION;
};


VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.Pos = mul(float4(input.Pos, 1.0f), WVPMatrix);
	output.Tex = input.Tex;
	output.Normal = mul(input.Normal, WorldMatrix);
	output.WPos = mul(input.Pos, WorldMatrix);

	return output;
}