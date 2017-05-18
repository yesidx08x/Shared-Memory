cbuffer Matrices : register(b0)
{
	matrix view;
	matrix projection;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
};

struct GS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

static const float4 g_positions[4] =
{
	float4(-1.0f, 1.0f, 0, 1),
	float4(1.0f, 1.0f, 0, 1),
	float4(-1.0f, -1.0f, 0, 1),
	float4(1.0f, -1.0f, 0, 1)
};

static const float2 g_uvs[4] =
{
	float2(0, 1),
	float2(1, 1),
	float2(0, 0),
	float2(1, 0),
};

[maxvertexcount(4)]
void main(point VS_OUT input[1], inout TriangleStream< GS_OUT > gsOutput)
{
	GS_OUT output;

	float4 viewPosition = mul(input[0].pos, view);

	for (int i = 0; i < 4; i++)
	{
		output.pos = mul(viewPosition + g_positions[i], projection);
		output.uv = g_uvs[i];

		gsOutput.Append(output);
	}
}