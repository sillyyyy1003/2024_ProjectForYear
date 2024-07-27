struct VS_IN
{
    float3 pos : POSITION0;
    float4 color : COLOR;
};

struct VS_OUT
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR;
};

cbuffer WVP : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
}

VS_OUT main (VS_IN vin)
{
    VS_OUT vout;
    vout.pos = float4(vin.pos, 1.0f);
    vout.pos = mul(vout.pos, world);
    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, projection);

    vout.color = vin.color; // alpha=1.0f
    return vout;
}