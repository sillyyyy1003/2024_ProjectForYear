struct PS_IN
{
	float4 pos : SV_POSITION0;
};

float4 main(PS_IN pin) : SV_TARGET
{
	float depth = pin.pos.z;
	return float4(depth, 0.f, 0.f, 1.f);

}