struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 worldPos : POSITION;
	float2 tex : TEXCOORD;
	float4 normal : NORMAL;
	float4 tangent : TANGENT;
	float4 bitangent : BITANGENT;
};

struct Light
{
	float4 lColor;
	float4 lDirection;
	float4 lPos;
	float4 lIntensity;
	int4 active;
};

cbuffer Lights : register(b0)
{
	Light light[20];
}

cbuffer CameraInfo : register(b1)
{
	float4 camPos;
	float4 camDirection;
}

cbuffer Matrices : register(b2)
{
	matrix view;
	matrix projection;
};

Texture2D albedomap : register(t0);
Texture2D roughnessmap : register(t1);
Texture2D metallicmap : register(t2);
Texture2D normalmap : register(t3);
TextureCube radiencemap : register(t5);
TextureCube irradiencemap : register(t6);
Texture2D ambientocclusionmap : register(t7);
SamplerState samp : register(s0);

static const float PI = 3.14159265f;

float3 ReadNormalMap(float3 normal, float3 tangent, float3 bitangent, float2 uv)
{
	normal = normalize(normal);
	tangent = normalize(tangent);
	tangent = normalize(tangent - dot(tangent, normal) * normal);
	bitangent = cross(tangent, normal);
	float3 mapNormal = normalmap.Sample(samp, uv).xyz;
	mapNormal = 2.0f * mapNormal - 1.0f;
	float3x3 tbn = float3x3(tangent, bitangent, normal);
	return normalize(mul(mapNormal, tbn)).xyz;
}

float3 CalcBumpedNormal(VS_OUT indata)
{
	float3 Normal = normalize(indata.normal);
	float3 Tangent = normalize(indata.tangent);
	Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);
	float3 Bitangent = normalize(cross(Tangent, Normal));
	float3 BumpMapNormal = normalmap.Sample(samp, indata.tex).xyz;  //texture(gNormalMap, TexCoord0).xyz;
																			//return BumpMapNormal;
	BumpMapNormal = 2.0 * BumpMapNormal - float3(1.0, 1.0, 1.0);
	float3 NewNormal;
	float3x3 TBN = float3x3(Tangent, Bitangent, Normal);
	NewNormal = mul(BumpMapNormal, TBN);
	NewNormal = normalize(NewNormal);
	return NewNormal;
}

float3 GammaCorrection(float3 color, float gamma)
{
	return pow(abs(color), gamma);
}

float3 F_Schlick(in float3 f0, in float f90, in float u)
{
	return f0 + (f90 - f0) * pow(1.0f - u, 5.0f);
}

float3 Cook_TorranceSpecular(float3 n, float3 v, float3 l, float3 h, float roughness, float3 f0)
{
	float NdotV = max(dot(n, v), 0.0f);
	float NdotL = max(dot(n, l), 0.0f);
	float LdotH = max(dot(l, h), 0.0f);
	float NdotH = max(dot(n, h), 0.0f);
	float VdotH = max(dot(v, h), 0.0f);

	// F
	float3 F = F_Schlick(f0, 1.0f, VdotH);

	// D
	float roughness2 = roughness * roughness;
	float f = (NdotH * roughness2 - NdotH) * NdotH + 1.0f;
	float D = roughness2 / (f * f);

	// G
	float g1 = (2.0f * (NdotH * NdotV)) / VdotH;
	float g2 = (2.0f * (NdotH * NdotL)) / VdotH;
	float G = min(1.0f, min(g1, g2));

	return (F * D * G) / (4 * NdotV * NdotL);
}

float4 main(VS_OUT input) : SV_TARGET
{
	float3 n = ReadNormalMap(input.normal, input.tangent, input.bitangent, input.tex).xyz;
	float3 v = normalize(camPos.xyz - input.worldPos.xyz);
	float3 r = normalize(reflect(-v, n)).xyz;
	float NdotV = saturate(dot(n, v));

	float3 radience = radiencemap.Sample(samp, r);
	float3 irradience = irradiencemap.Sample(samp, input.worldPos.xyz);
	float4 albedo = albedomap.Sample(samp, input.tex);
	float roughness = roughnessmap.Sample(samp, input.tex).r;
	float metallic = metallicmap.Sample(samp, input.tex).r;

	float3 diffuseColor = lerp(albedo.rgb, 0.0f, metallic);
	float3 specularColor = lerp(0.04, albedo.rgb, metallic);

	// Gamma correction
	radience = GammaCorrection(radience, 2.2f);
	irradience = GammaCorrection(irradience, 2.2f);
	diffuseColor = GammaCorrection(albedo, 2.2f);
	roughness = GammaCorrection(roughness, 2.2f);
	metallic = GammaCorrection(metallic, 2.2f);

	float3 Cook_TorranceS = float3(0.0f, 0.0f, 0.0f);
	float3 lambertD = float3(0.0f, 0.0f, 0.0f);

	// Ambient
	lambertD += irradience * diffuseColor;

	// Ambient specular
	float3 fresnel = F_Schlick(specularColor, 1.0f, NdotV);

	for (int i = 0; i < 20; i++)
	{
		if (light[i].active.x == 1)
		{
			float3 l = normalize(light[i].lPos - input.worldPos.xyz);
			float NdotL = saturate(dot(n, l));
			float3 h = normalize(v + l);

			// Diffuse
			lambertD += (NdotL * diffuseColor * light[i].lColor * light[i].lIntensity.x) / PI;

			// Specular
			if (NdotL > 0.0f)
			{
				Cook_TorranceS += Cook_TorranceSpecular(n, v, l, h, roughness, specularColor) * specularColor * light[i].lColor * light[i].lIntensity.x;
			}
		}
	}
	
	// Composit
	float3 output = float3(0.0f, 0.0f, 0.0f);
	output = lerp(lambertD, radience, fresnel) + Cook_TorranceS;
	output = GammaCorrection(output, 1.0f / 2.2f);

	return float4(output, 1.0f);



	//float3 n = CalcBumpedNormal(input);
	//float3 l = normalize(lPos.xyz - input.worldPos.xyz);
	//float3 v = normalize(camPos - input.worldPos);
	//float3 r = normalize(reflect(v, n)).xyz;
	//float3 h = normalize(v + l);
	//float NdotV = saturate(dot(n, v));
	//float NdotL = saturate(dot(n, l));
	//float LdotH = saturate(dot(l, h));
	//float NdotH = saturate(dot(n, h));

	//float3 radiance = float3(0.0f, 0.0f, 0.0f);
	//radiance = radiencemap.Sample(samp, r);
	//float3 irr = irradiencemap.Sample(samp, input.worldPos.xyz);
	//irr = float3(0.5, 0.5, 0.5);
	//float4 albedo = albedomap.Sample(samp, input.tex);
	//float rough = roughnessmap.Sample(samp, input.tex).r;
	//float metal = metallicmap.Sample(samp, input.tex).r;

	//float3 diffuseColor = lerp(albedo.rgb, 0.0f, metal);
	//float3 specularColor = lerp(0.04, albedo.rgb, metal);

	//// Gamma correction
	//radiance = pow(radiance, 2.2f);
	//irr = pow(irr, 2.2f);
	//diffuseColor = pow(albedo, 2.2f);
	//rough = pow(rough, 2.2f);
	//metal = pow(metal, 2.2f);

	//// Diffuse
	//float3 lambertD = (NdotL * diffuseColor * lColor.rgb * lIntensity.x) / PI;

	//// Ambient
	//lambertD += irr * diffuseColor;

	//// Ambient specular
	//float3 fresnel = F_Schlick(specularColor, 1.0f, NdotV);

	//// Specular
	//float3 Cook_TorranceS = float3(0.0f, 0.0f, 0.0f);
	//if (NdotL > 0.0f)
	//	Cook_TorranceS = Cook_TorranceSpecular(n, v, l, h, rough, specularColor) * specularColor * lColor.rgb * lIntensity.x;

	//// Composit
	//float3 output = float3(0.0f, 0.0f, 0.0f);
	//output = lerp(lambertD, radiance, fresnel);
	//output += Cook_TorranceS;
	//output = pow(output, 1.0f / 2.2f);

	//return float4(output, 1.0f);
}