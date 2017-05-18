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
	float4 worldPos : POSITION;
	float2 tex : TEXCOORD;
	float4 normal : NORMAL;
	float4 tangent : TANGENT;
	float4 bitangent : BITANGENT;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.pos = mul(float4(input.pos, 1.0f), mul(transform, mul(view, projection)));
	output.worldPos = mul(float4(input.pos, 1.0f), transform);
	output.tex = input.tex;
	output.normal = mul(float4(input.normal, 0.0f), transform);
	output.tangent = mul(float4(input.tangent, 0.0f), transform);
	output.bitangent = mul(float4(input.bitangent, 0.0f), transform);

	return output;
}