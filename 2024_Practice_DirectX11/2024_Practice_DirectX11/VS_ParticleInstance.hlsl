struct VS_IN
{
	//Vertex Buffer
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD;
	//float4 color : COLOR;

	//Instance Buffer
	float4 color : COLOR;
	float4x4 WorldMatrix : INSTANCE_MTX;

};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
	float2 tex : TEXCOORD;
};
 
cbuffer ParticleConstantBuffer : register(b0)
{
	float4x4 view;
	float4x4 proj;

};

VS_OUT main(VS_IN vin)
{
	VS_OUT vOut;
	//ビルボード機能実現
	float3 right = float3(view._11, view._21, view._31); //カメラのRightAxis
	float3 up = float3(view._12, view._22, view._32); //カメラのUpAxi

	float4 vertexPos = mul(float4(0.f, 0.f, 0.f, 1.0f), vin.WorldMatrix); //Quad中心
	
	float scaleX = length(float3(vin.WorldMatrix._11, vin.WorldMatrix._12, vin.WorldMatrix._13));
	float scaleY = length(float3(vin.WorldMatrix._21, vin.WorldMatrix._22, vin.WorldMatrix._23));

	float3 worldPosition = vertexPos.xyz + right * vin.pos.x * scaleX + up * vin.pos.y * scaleY; //Quad位置
	vOut.pos = float4(worldPosition, 1.0f);
	vOut.pos = mul(vOut.pos, view);
	vOut.pos = mul(vOut.pos, proj);

	vOut.color = vin.color;
	vOut.tex = vin.tex;

	return vOut;

}