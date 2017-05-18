struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 noMatrixPos : POSITION;
};

TextureCube cubemap : register(t5);
SamplerState samp : register(s0);

float4 main(VS_OUT input) : SV_TARGET
{
	return cubemap.Sample(samp, input.noMatrixPos.xyz);
}