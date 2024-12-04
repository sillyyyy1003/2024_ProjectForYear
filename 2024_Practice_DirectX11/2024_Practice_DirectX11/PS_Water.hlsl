//Use Lambert Model
#include "LightHelper.hlsli"
struct PS_IN
{
	float4 pos : SV_POSITION0;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD;
	float4 worldPos : POSITION0;
};

cbuffer NormalConstantBuffer : register(b0)
{
	//Camera pos
	float4 cameraPos;

	//Environment light
	float4 lightAmbient;
	float4 lightDiffuse;
	float4 lightPos;

	//Material
	Material material;

}

//追加のポイントライト
cbuffer PointLight : register(b1)
{
	PointLight pointLight[2]; // Define NUM_POINT_LIGHTS as the max number
}

Texture2D myTex : register(t0);
SamplerState mySampler : register(s0);

float3 PointLightCal(PointLight pointLight, float3 pos, float3 normal, float3 toEyeW_Unit)
{
	float3 color = { 0, 0, 0 };
	if (pointLight.isEnable == 0)
		return color;

	//calculate vertex to pointLight vector
	float3 toLightVec = pointLight.position - pos;
	float3 V = normalize(toLightVec);
	float3 toLightLen = length(toLightVec);

	float3 N = normalize(normal);
	float dotNV = saturate(dot(N, V));
	float3 attenuation = saturate(1.0f - toLightLen / pointLight.range);
	attenuation *= pointLight.att.x;
	attenuation = pow(attenuation, 2.f);

	color = pointLight.diffuse.rgb * dotNV * attenuation;
	return color;
}


float4 main(PS_IN pin, bool frontFace : SV_IsFrontFace) : SV_TARGET
{
	float4 color = float4(0, 0, 0, 0);

	color = myTex.Sample(mySampler, pin.tex);
	clip(color.a - 0.1f);

	float3 N = normalize(pin.normal);
	float3 toEye = normalize(-cameraPos.xyz);

	//環境光計算
	float4 ambient = material.ambient * lightAmbient * 0.2f;
	float3 lightVec = normalize(lightPos.xyz);

    //Lambert Diffuse計算
	float diffuseFactor = saturate(dot(lightVec, N)) * 0.5f + 0.5f;
	float4 diffuse = diffuseFactor * lightDiffuse * material.diffuse;

    // specular 計算
	float3 V = normalize(cameraPos.xyz - pin.worldPos.xyz);
	float specFactor = pow(max(dot(V, toEye), 0.0f), material.specular.w);
	float4 spec = specFactor * material.specular;

    //Point Light Calculation
	for (int i = 0; i < MAX_NUM_POINT_LIGHT; i++)
		diffuse.rgb += PointLightCal(pointLight[i], pin.worldPos.xyz, N, toEye);

	float4 litColor = color * (ambient + diffuse * 2.0f) + spec;

    //アルファ値をマテリアルに計算
	litColor.a = material.diffuse.a;
	return litColor;
}