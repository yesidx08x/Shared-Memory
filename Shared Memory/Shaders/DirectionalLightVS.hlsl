cbuffer Matrices : register(b0)
{
	matrix view;
	matrix projection;
};

cbuffer Transform : register(b1) { matrix transform; };

struct VS_IN
{
	float3 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.pos = mul(float4(input.pos, 1.0f), mul(transform, mul(view, projection)));
	output.tex = input.tex;

	return output;
}