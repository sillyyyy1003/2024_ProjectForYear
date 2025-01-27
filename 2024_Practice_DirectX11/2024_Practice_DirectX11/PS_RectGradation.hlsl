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
   
	float2 diff = abs(pin.tex - float2(0.5f, 0.5f));//�����v�Z
	float maxDist = max(diff.x, diff.y); //�ő勗��
	float distance = maxDist /diff.x;//�������O�`�P�ɐ���

	float4 color = material.diffuse * (1 - distance);//�F�v�Z
	clip(color.a - 0.1f);

	return color;
}