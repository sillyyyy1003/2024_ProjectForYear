#include "LightHelper.hlsli"

struct PS_IN
{
	float4 pos : SV_POSITION0;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD;
};

cbuffer Material : register(b0)
{
	Material material;
}

cbuffer FadeParam:register(b1)
{
	float2 screenCenter;
	float intensity;
	float radius;
}

Texture2D myTex : register(t0);
SamplerState mySampler : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{

	float2 diff = pin.tex - screenCenter;
	float distance = length(diff);
	float alpha = distance * radius;
	return float4(1.0, 1.0, 1.0, alpha);
}