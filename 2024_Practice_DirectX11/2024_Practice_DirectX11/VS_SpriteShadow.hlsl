struct VS_IN
{
	float3 pos : POSITION0;
	float2 uv : TEXCOORD0;
	
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
	float4 screenPos : POSITION0;
};

cbuffer WVP : register(b0)
{
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

cbuffer LightMatrix : register(b2)
{
	float4x4 LWorld;
	float4x4 LView;
	float4x4 LProj;
};

VS_OUT main(VS_IN vin)
{
	VS_OUT vOut;
	vOut.pos = float4(vin.pos, 1.0f);
	vOut.pos.xy *= size;
	vOut.pos.xy += offset;

	float4 pos = vOut.pos;

	//Game Camera
	vOut.pos = mul(vOut.pos, world);
	vOut.pos = mul(vOut.pos, view);
	vOut.pos = mul(vOut.pos, proj);

	//Light Camera
	pos = mul(pos, LWorld);
	pos = mul(pos, LView);
	pos = mul(pos, LProj);
	vOut.screenPos = pos;

	//UV
	vOut.uv = vin.uv;
	vOut.uv *= uvScale;
	vOut.uv += uvPos;
	vOut.color = color;

	return vOut;
}