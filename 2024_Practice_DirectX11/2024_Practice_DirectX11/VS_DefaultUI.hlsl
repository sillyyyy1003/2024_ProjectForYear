struct VS_IN
{
    float3 pos : POSITION0;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
};

struct VS_OUT
{
    float4 pos : SV_POSITION0;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;

};

cbuffer WVP : register(b0)
{
    matrix world;
    matrix view;
    matrix proj;

}


VS_OUT main (VS_IN vin)
{
    VS_OUT vOut;
    vOut.pos = float4(vin.pos, 1.0f);

    vOut.pos = mul(vOut.pos, world);
    vOut.pos = mul(vOut.pos, view);
    vOut.pos = mul(vOut.pos, proj);

	vOut.normal = vin.normal;
	vOut.tex = vin.tex;
    return vOut;
}