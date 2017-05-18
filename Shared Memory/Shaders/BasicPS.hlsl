struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};

Texture2D albedo : register(t0);
SamplerState samp : register(s0);

float4 main(VS_OUT input) : SV_TARGET
{
	float4 textureColor = float4(1.0, 1.0, 1.0, 1.0);
	textureColor = albedo.Sample(samp, input.tex);
	return textureColor;
}