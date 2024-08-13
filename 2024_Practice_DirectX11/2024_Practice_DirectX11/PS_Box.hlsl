//Use Lambert Model
static const float PI = 3.1415926f; // ��
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

cbuffer Light: register(b2){
    float4 lightAmbient;
	float4 lightDiffuse;
    float4 lightDir;
}

Texture2D myTex : register(t0);
SamplerState mySampler : register(s0);

float4 main (PS_IN pin) : SV_TARGET
{
    float4 color = float4(0, 0, 0, 0);
    if(material.isTexEnable)
    {
        color = myTex.Sample(mySampler, pin.tex);
    }else
    {
		color = material.diffuse;
    }


    float3 normal = normalize(pin.normal);
    float3 toEye = normalize(-eyePos);

	//�����v�Z
    float4 ambient = material.ambient * lightAmbient;
    float3 lightVec = normalize(lightDir);
    //Lambert Diffuse�v�Z
    float diffuseFactor = saturate(dot(lightVec, normal));
    float4 diffuse = diffuseFactor * lightDiffuse;
    // specular �v�Z
    float3 v = normalize(eyePos.xyz - pin.worldPos.xyz);
    float specFactor = pow(max(dot(v, toEye), 0.0f), material.specular.w);
    float4 spec = specFactor * material.specular;

	float4 litColor = color * (ambient + diffuse) + spec;
    litColor = saturate(litColor);
    //�A���t�@�l���}�e���A���Ɍv�Z
    litColor.a = material.diffuse.a * color.a;
     return litColor;

}