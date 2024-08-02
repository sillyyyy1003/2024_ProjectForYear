//Use Lambert Model
static const float PI = 3.1415926f; // ��
#include "LightHelper.hlsli"
struct PS_IN
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR;
    float3 normal : NORMAL;
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


float4 main (PS_IN pin) : SV_TARGET
{
    float4 color = material.diffuse;

    float3 N = normalize(pin.normal); // �@���x�N�g���𐳋K��
    float3 L = normalize(lightDir.xyz); // ���C�g�̕����𐳋K��

    float diffuse = saturate(dot(N, L)); // �g�U���ˌ��̌v�Z(�@���x�N�g���ƃ��C�g�̕����̓���)
    float4 lambertDiffuse = diffuse * lightDiffuse / PI; //todo:Check if is right;
	//color *= diffuse * lightDiffuse/PI + lightAmbient; //�J���[�Ɋg�U���ˌ��Ɗ�����K�p
    color *= lambertDiffuse + lightAmbient;

    //�����x�N�g���Ɣ��˃x�N�g���̌v�Z
    float3 V = normalize(eyePos.xyz - pin.worldPos.xyz);
    float3 R = normalize(reflect(-L, N));
    //���ʌ��̌v�Z
    float specular = saturate(dot(V, R));
    //�J���[�ɋ��ʌ������Z
    float specPower = material.specular.x;
    color += pow(specular, specPower) * material.specular.w;
    //�A���t�@�l��1.0�ɐݒ�
    color.a = 1.0f;
    return color;

}