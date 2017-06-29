struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 noMatrixPos : POSITION;
	float2 tex : TEXCOORD;
	float4 normal : NORMAL;
	float4 tangent : TANGENT;
	float4 bitangent : BITANGENT;
};

cbuffer Light : register(b0)
{
	float4 lColor;
	float4 lDirection;
	float4 lPos;
	float4 lIntensity;
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

Texture2D albedo : register(t0);
Texture2D normalmap : register(t3);
SamplerState samp : register(s0);

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

float4 main(VS_OUT input) : SV_TARGET
{
	float4 tex = albedo.Sample(samp, input.tex);
	float4 ka = float4(tex.xyz * 0.25f, tex.w);

	float3 n = ReadNormalMap(input.normal.xyz, input.tangent.xyz, input.bitangent.xyz, input.tex);
	n = normalize(input.normal.xyz);
	float3 l = normalize(-lDirection.xyz);
	float3 v = normalize(camPos.xyz - input.noMatrixPos.xyz);
	//v = normalize(camDirection);
	float3 r = reflect(-l, n);
	float3 h = normalize(l + v);
	float3 lc = saturate(lColor.xyz);

	float NdotL = max(dot(n, l), 0.0f);
	float NdotH = max(dot(n, h), 0.0f);
	float RdotV = max(dot(r, v), 0.0f);

	float3 kd = float3(tex.xyz * NdotL * lc * lIntensity.x) * NdotL;
	float3 ks = float3(pow(NdotH, 200.0f) * lc * lIntensity.x) * NdotL; // Blinn - Phong
	return float4(saturate(kd + ka.xyz + ks), tex.w);
}