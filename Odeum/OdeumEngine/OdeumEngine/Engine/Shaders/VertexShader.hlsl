cbuffer VSConstants : register(b0)
{
	matrix viewProj;
	matrix model;
	float3 viewerPos;
};

struct VS_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float3 worldPos : WorldPos;
	float2 uv : TexCoord0;
	float3 viewDir : TexCoord1;
	float3 normal : Normal;
	float3 tangent : Tangent;
	float3 bitangent : Bitangent;
};

PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output;

	float4 pos = float4(input.position, 1.0f);

	pos = mul(pos, model);
	output.worldPos = pos;
	pos = mul(viewProj, pos);

	output.position = pos;
	output.uv = input.uv;
	output.viewDir = output.worldPos - viewerPos;
	output.normal = mul(input.normal, model);
	output.tangent = input.tangent;
	output.bitangent = input.bitangent;

	return output;
}