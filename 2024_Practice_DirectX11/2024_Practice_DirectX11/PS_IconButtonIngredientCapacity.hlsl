#include "LightHelper.hlsli"
/// @brief Default Pixel Shader for UI
/// No light Effect
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

cbuffer IngredientIconCBuffer : register(b1)
{
	float4 pigmentColor;
	float capacity;		//Œ»Ý‚ÌƒLƒƒƒpƒVƒeƒB
}

Texture2D myTex : register(t0);
SamplerState mySampler : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	
	//tex‚Íã‚©‚çi‚O`‚Pj‚Ì‚½‚ßA”»’è‚Í‹t‚É‚·‚é
	float4 color = myTex.Sample(mySampler, pin.tex);
	color.rgb = pin.tex.y >= capacity ? pigmentColor.rgb :	material.diffuse.rgb;
	clip(color.a - 0.1f);
	return color;

}