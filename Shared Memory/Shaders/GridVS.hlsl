cbuffer Matrices : register(b0)
{
	matrix view;
	matrix projection;
};

struct VS_IN
{
	float3 pos : SV_POSITION;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;
	output.pos = mul(float4(input.pos, 1.0f), mul(view, projection));
	return output;
}