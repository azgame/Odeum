struct PS_INPUT
{
	float4 position : SV_POSITION;
	float4 worldPos : WorldPos;
};

float4 main(PS_INPUT input) : SV_Target0
{
	return float4(1.0f, 0.2f, 0.6f, 1.0f);
}