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
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 noMatrixPos : POSITION;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.pos = mul(float4(input.pos, 1.0f), mul(transform, mul(view, projection)));
	output.noMatrixPos = float4(input.pos, 1.0f);

	return output;
}