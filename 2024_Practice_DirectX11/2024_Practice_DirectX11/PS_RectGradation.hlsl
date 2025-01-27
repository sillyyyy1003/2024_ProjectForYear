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

SamplerState mySampler : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
   
	float2 diff = abs(pin.tex - float2(0.5f, 0.5f));//‹——£ŒvZ
	float maxDist = max(diff.x, diff.y); //Å‘å‹——£
	float distance = maxDist /diff.x;//‹——£‚ğ‚O`‚P‚É§ŒÀ

	float4 color = material.diffuse * (1 - distance);//FŒvZ
	clip(color.a - 0.1f);

	return color;
}