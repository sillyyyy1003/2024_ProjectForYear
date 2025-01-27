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

cbuffer GradationRadius : register(b1)
{
	float radius;
}

SamplerState mySampler : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	float2 diff = pin.tex - float2(0.5f, 0.5f);//中心点までの差を計算
	float distance = radius * length(diff) / (0.5f * sqrt(2.0f)); //中心点との距離を計算;

	float4 color = material.diffuse * (1 - distance);
	clip(color.a - 0.1f);
	return color;

}