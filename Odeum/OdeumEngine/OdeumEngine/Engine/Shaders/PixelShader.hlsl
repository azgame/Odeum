struct PS_INPUT
{
	float4 position : SV_POSITION;
	float4 colour : Colour;
};

float4 main(PS_INPUT input) : SV_Target0
{
	return input.colour;
}