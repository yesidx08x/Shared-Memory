cbuffer Transform : register(b1) { matrix transform; };

struct VS_IN
{
	float4 pos : SV_POSITION;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;
	output.pos = mul(input.pos, transform);
	return output;
}