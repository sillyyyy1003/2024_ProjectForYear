struct VS_IN
{
	float3 pos : POSITION0;
	float2 uv : TEXCOORD0;
};
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : TEXCOORD1;
};

cbuffer WVP : register(b0) {
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};

cbuffer Param : register(b1)
{
	float2 offset;
	float2 size;
	float2 uvPos;
	float2 uvScale;
	float4 color;
};

VS_OUT main(VS_IN vin)
{
	VS_OUT vOut;
	vOut.pos = float4(vin.pos, 1.0f);
	vOut.pos.xy *= size;
	vOut.pos.xy += offset;
	vOut.pos = mul(vOut.pos, world);
	vOut.pos = mul(vOut.pos, view);
	vOut.pos = mul(vOut.pos, proj);
	vOut.uv = vin.uv;
	vOut.uv *= uvScale;
	vOut.uv += uvPos;
	vOut.color = color;
	return vOut;
}
