Texture2D objectTexture : register(t0);
SamplerState Sampler;

cbuffer data : register(b0)
{
	float3 eyePos;
	float roughnessValue; // 0 : smooth, 1: rough
	float3 lightPosition;
	float F0; // fresnel reflectance at normal incidence
	float3 pad;
	float k; // fraction of diffuse reflection (specular reflection = 1 - k)
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
	float2 Tex : TEXCOORD;
	float3 Normal : NORMAL;
	float3 WPos : POSITION;
};

float4 PS_main(VS_OUT input) : SV_Target
{
	float3 normal = normalize(input.Normal);
	float3 lightDirection = normalize(lightPosition - input.WPos);
	// do the lighting calculation for each fragment.
	float NdotL = max(dot(normal, lightDirection), 0.0f);

	float specular = 0.0f;
	if (NdotL > 0.0f)
	{
		float3 eyeDir = normalize(eyePos - input.WPos);

		// calculate intermediary values
		float3 halfVector = normalize(lightDirection + eyeDir);
		float NdotH = max(dot(normal, halfVector), 0.0f);
		float NdotV = max(dot(normal, eyeDir), 0.0f); // note: this could also be NdotL, which is the same value
		float VdotH = max(dot(eyeDir, halfVector), 0.0f);
		float mSquared = roughnessValue * roughnessValue;

		// geometric attenuation
		float NH2 = 2.0f * NdotH;
		float g1 = (NH2 * NdotV) / VdotH;
		float g2 = (NH2 * NdotL) / VdotH;
		float geoAtt = min(1.0f, min(g1, g2));

		// roughness (or: microfacet distribution function)
		// beckmann distribution function
		float r1 = 1.0f / (4.0f * mSquared * pow(NdotH, 4.0f));
		float r2 = (NdotH * NdotH - 1.0f) / (mSquared * NdotH * NdotH);
		float roughness = r1 * exp(r2);

		// fresnel
		// Schlick approximation
		float fresnel = pow(1.0f - VdotH, 5.0f);
		fresnel *= (1.0f - F0);
		fresnel += F0;

		specular = (fresnel * geoAtt * roughness) / (NdotV * NdotL * 3.14f);
	}

	float3 finalValue = objectTexture.Sample(Sampler, input.Tex).xyz * ambientColor * NdotL * diffuseColor * (k + specular * (1.0f - k)) * specularColor;
	return float4(finalValue, 1.0f);
};