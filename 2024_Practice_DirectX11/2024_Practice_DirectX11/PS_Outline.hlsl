struct PS_IN
{
	float4 pos : SV_POSITION0;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD;
	float4 worldPos : POSITION0;
};


Texture2D myTex : register(t0);
SamplerState mySampler : register(s1);

float4 main(PS_IN pin):SV_TARGET
{
	float4 color = myTex.Sample(mySampler, pin.tex);
	return color;
}