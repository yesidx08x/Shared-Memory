struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 noMatrixPos : POSITION;
	float2 tex : TEXCOORD;
	float4 normal : NORMAL;
};

cbuffer Light : register(b0)
{
	float4 lColor;
	float4 lDirection;
	float4 lPos;
	float4 lIntensity;
}

Texture2D albedo : register(t0);
SamplerState samp : register(s0);

float4 main(VS_OUT input) : SV_TARGET
{
	float4 tex = albedo.Sample(samp, input.tex);
	float4 ka = float4(tex.xyz * 0.25f, tex.w);
	
	float4 n = normalize(input.normal);
	float4 l = normalize(-lDirection);

	float4 kd = float4(saturate(tex.xyz * dot(l, n) * lColor.xyz) * lIntensity.x, tex.w);
	return kd + ka;
}