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

cbuffer UiStatus : register(b1)
{
    int isActive;
	int dummy1, dummy2, dummy3;
}

Texture2D myTex : register(t0);
SamplerState mySampler : register(s0);

float4 main (PS_IN pin) : SV_TARGET
{
    float4 color = float4(0, 0, 0, 0);

    if (material.isTexEnable)
    {
        color = myTex.Sample(mySampler, pin.tex);
    }
    else
    {
        color = material.diffuse;
    }

	clip(color.a - 0.1f);

    //Change Factor according to ui states
    float4 ambientFactor = float4(1.0, 1.0, 1.0, 1.0f);

    return color  * ambientFactor;

}