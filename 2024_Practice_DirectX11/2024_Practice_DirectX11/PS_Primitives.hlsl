//Use Lambert Model
#include "LightHelper.hlsli"
struct PS_IN
{
    float4 pos : SV_POSITION0;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
    float4 worldPos : POSITION0;
};

cbuffer Material : register(b0)
{
    Material material;
}

cbuffer Camera : register(b1)
{
    float4 eyePos;
}

//Ambient light
cbuffer DirLight : register(b2)
{
    float4 lightAmbient;
    float4 lightDiffuse;
    float4 lightPos;
}

//追加のポイントライト
cbuffer PointLight:register(b3)
{
	PointLight pointLight[MAX_NUM_POINT_LIGHT]; // Define NUM_POINT_LIGHTS as the max number
	int actualLightNum;//actual light number
	int dummy1, dummy2, dummy3;
}

Texture2D myTex : register(t0);
SamplerState mySampler : register(s0);

float4 main(PS_IN pin, bool frontFace : SV_IsFrontFace) : SV_TARGET
{
    float4 color = float4(0, 0, 0, 0);

    if (material.isTexEnable)
    {
        color = myTex.Sample(mySampler, pin.tex);
        clip(color.a-0.1f);
    }
    else
    {
        color = material.diffuse;
    }


    float3 normal = normalize(pin.normal);

    if(!frontFace)
    {
		normal = -normal;
	}

    float3 toEye = normalize(-eyePos.xyz);

	//環境光計算
	float4 ambient = material.ambient * lightAmbient * 0.2f;
    float3 lightVec = normalize(lightPos.xyz);

    //Lambert Diffuse計算
	float diffuseFactor = saturate(dot(lightVec, normal)) * 0.5f + 0.5f;
	float4 diffuse = diffuseFactor * lightDiffuse * material.diffuse;

    // specular 計算
    float3 v = normalize(eyePos.xyz - pin.worldPos.xyz);
    float specFactor = pow(max(dot(v, toEye), 0.0f), material.specular.w);
    float4 spec = specFactor * material.specular;

    float4 litColor = color * (ambient + diffuse) + spec;
    litColor = saturate(litColor);

    //アルファ値をマテリアルに計算
    litColor.a = material.diffuse.a;
    return litColor;

}