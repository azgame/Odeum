
cbuffer VSConstants : register(b0)
{
	matrix viewProj;
	matrix model;
	float3 viewerPos;
};

cbuffer AnimConstants : register(b3)
{
	float4x4 inBoneTransforms[50];
};

struct AnimBuffer
{
	float4 boneIds;
	float4 boneWeights;
};

StructuredBuffer<AnimBuffer> animBuffer : register(t4);

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

PS_INPUT main(VS_INPUT input, uint Vertex_ID : SV_VertexID)
{
	PS_INPUT output;

	//float4 weights = normalize(animBuffer[Vertex_ID].boneWeights);
	float4 weights = animBuffer[Vertex_ID].boneWeights;

	matrix boneTransform = { {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0} };
	boneTransform += inBoneTransforms[(int)animBuffer[Vertex_ID].boneIds.x] * weights.x;
	boneTransform += inBoneTransforms[(int)animBuffer[Vertex_ID].boneIds.y] * weights.y;
	boneTransform += inBoneTransforms[(int)animBuffer[Vertex_ID].boneIds.z] * weights.z;
	boneTransform += inBoneTransforms[(int)animBuffer[Vertex_ID].boneIds.w] * weights.w;

	float4 pos = mul(boneTransform, float4(input.position, 1.0));

	output.worldPos = mul(pos, model);
	output.position = mul(viewProj, mul(pos, model));
	output.uv = input.uv;
	output.viewDir = output.worldPos - viewerPos;
	output.normal = mul(input.normal, mul(model, boneTransform));
	output.tangent = input.tangent;
	output.bitangent = input.bitangent;

	return output;
}