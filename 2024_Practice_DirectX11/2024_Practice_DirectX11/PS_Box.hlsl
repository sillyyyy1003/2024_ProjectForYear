//Use Lambert Model
static const float PI = 3.1415926f; // π
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

    float3 N = normalize(pin.normal); // 法線ベクトルを正規化
    float3 L = normalize(lightDir.xyz); // ライトの方向を正規化

    float diffuse = saturate(dot(N, L)); // 拡散反射光の計算(法線ベクトルとライトの方向の内積)
    float4 lambertDiffuse = diffuse * lightDiffuse / PI; //todo:Check if is right;
	//color *= diffuse * lightDiffuse/PI + lightAmbient; //カラーに拡散反射光と環境光を適用
    color *= lambertDiffuse + lightAmbient;

    //視線ベクトルと反射ベクトルの計算
    float3 V = normalize(eyePos.xyz - pin.worldPos.xyz);
    float3 R = normalize(reflect(-L, N));
    //鏡面光の計算
    float specular = saturate(dot(V, R));
    //カラーに鏡面光を加算
    float specPower = material.specular.x;
    color += pow(specular, specPower) * material.specular.w;
    //アルファ値を1.0に設定
    color.a = 1.0f;
    return color;

}