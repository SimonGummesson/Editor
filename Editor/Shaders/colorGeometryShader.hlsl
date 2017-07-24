cbuffer WVPMatrix : register(b0)
{
	matrix WVPMatrix;
	matrix WorldMatrix;
};

struct VS_OUT
{
	float3 Pos : POSITION;
	float3 Color : COLOR;
};

struct GS_OUT
{
	float4 Pos : SV_POSITION;
	float3 WPos : POSITION;
	float3 Color : COLOR;
	float3 Normal : NORMAL;
};

[maxvertexcount(3)]
void GS_main(triangle VS_OUT input[3] : SV_POSITION, inout TriangleStream< GS_OUT > output)
{
	float3 normal = normalize(cross(input[1].Pos - input[0].Pos, input[2].Pos - input[0].Pos));

	for (uint i = 0; i < 3; i++)
	{
		GS_OUT element;
		element.WPos = mul(input[i].Pos, WorldMatrix);
		element.Normal = normal;
		element.Pos = mul(float4(input[i].Pos, 1.0f), WVPMatrix);
		element.Color = input[i].Color;
		output.Append(element);
	}
	output.RestartStrip();
}