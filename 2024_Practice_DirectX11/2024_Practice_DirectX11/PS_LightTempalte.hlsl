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

cbuffer Lights : register(b1)
{
    DirectionLight dirLight;
    PointLight pointLight;
    SpotLight spotLight;

}

cbuffer Camera : register(b2)
{
    float4 eyePos;
}

float4 main(PS_IN pin) : SV_TARGET
{
    // normalize
    pin.normal = normalize(pin.normal);

    // To Camera
    float3 toEyeW = normalize(eyePos - pin.worldPos);

    float4 ambient, diffuse, spec;
    float4 A, D, S;
    ambient = diffuse = spec = A = D = S = float4(0.0f, 0.0f, 0.0f, 0.0f);

    ComputeDirectionalLight(material, dirLight, pin.normal, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;

    ComputePointLight(material, pointLight, pin.worldPos, pin.normal, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;

    ComputeSpotLight(material, spotLight, pin.worldPos, pin.normal, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;

    float4 litColor = pin.color * (ambient + diffuse) + spec;
    litColor = saturate(litColor);

    litColor.a = material.diffuse.a * pin.color.a;
    
    return litColor;
}