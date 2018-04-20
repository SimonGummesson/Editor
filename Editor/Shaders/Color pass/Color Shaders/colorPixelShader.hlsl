ByteAddressBuffer lightDataByte : register(t0);
cbuffer cameraData : register(b0)
{
    float4 eyePos;
};

cbuffer lightData : register(b1)
{
	float3 ambientColor;
	float pad1;
	float3 diffuseColor;
	float pad2;
	float3 specularColor;
	float specularPower;
};

struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float3 WPos : POSITION;
    float3 Color : COLOR;
    float3 Normal : NORMAL;
};

float4 PS_main(VS_OUT input) : SV_Target
{
	float3 normal = normalize(input.Normal);
    float3 finalColor = asfloat(lightDataByte.Load3(16)) * ambientColor; // Ambient

    for (int i = 0; i < asint(lightDataByte.Load(0)); i++)
    {
        float3 lightDirection;
        int type = asint(lightDataByte.Load(32 + i * 4 * 16));
        float attenuation = 1.f;
        if (type == 0) // point light
        {
            lightDirection = normalize(asfloat(lightDataByte.Load3(32 + i * 4 * 16 + 16 * 3)) - input.WPos);
            float4 attData = asfloat(lightDataByte.Load4(32 + i * 4 * 16));
            attData.x = length(asfloat(lightDataByte.Load3(32 + i * 4 * 16 + 16 * 3)) - input.WPos);
            attenuation = 1.f / (1.f + attData.y * attData.x + attData.z * pow(attData.x, 2));
        }
        else if (type == 1) // directional light
            lightDirection = -normalize(asfloat(lightDataByte.Load3(32 + i * 4 * 16 + 16 * 3)));

        float NdotL = max(dot(normal, lightDirection), 0.f);

        float specular = pow(max(dot(normalize(eyePos.xyz - input.WPos), reflect(lightDirection, normal)), 0.f), specularPower);

        finalColor += (NdotL * asfloat(lightDataByte.Load3(32 + i * 4 * 16 + 16 * 1)) * diffuseColor
            + specular * asfloat(lightDataByte.Load3(32 + i * 4 * 16 + 16 * 2)) * specularColor)
            * input.Color * attenuation;
    }
	return float4(finalColor, 1.0f);
};