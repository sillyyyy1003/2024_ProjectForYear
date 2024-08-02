struct VS_IN
{
    float3 pos : POSITION0;
    float4 color : COLOR;
    float3 normal : NORMAL;
};

struct VS_OUT
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float4 worldPos : POSITION0;
};

cbuffer WVP : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
}

VS_OUT main (VS_IN vin)
{
    VS_OUT vOut;

    vOut.pos = float4(vin.pos, 1.0f);
    vOut.pos = mul(vOut.pos, world);
	// ピクセルシェーダーに渡すために、途中で変数にコピー
    vOut.worldPos = vOut.pos;

    vOut.pos = mul(vOut.pos, view);
    vOut.pos = mul(vOut.pos, proj);

    vOut.normal = mul(vin.normal, (float3x3) world);
    vOut.color = float4(1.f, 1.f, 1.f, 1.f);

    return vOut;
}