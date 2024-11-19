struct VS_IN
{
	float3 pos : POSITION0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT0;
	float2 tex : TEXCOORD;
};

struct VS_OUT
{
	float4 pos : SV_POSITION0;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL0;
	float4 tangentW : TANGENT;
	float4 worldPos : POSITION0;
};

cbuffer WVP : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
}

VS_OUT main(VS_IN vin)
{
/*
	VS_OUT vout;
	vout.pos = float4(vin.pos, 1.0f);
	vout.pos = mul(vout.pos, world);
	float4 worldPos = vout.pos; // 計算途中でワールド座標の情報を変数にコピー
	vout.worldPos = worldPos;
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);
	vout.tex = vin.tex;
	vout.normal = mul(vin.normal, (float3x3) world);

	// 接ベクトルと面法線から従法線(Bynormal)を計算
	float3 T = mul(vin.tangent, (float3x3) world);
	T = normalize(T);
	float3 N = normalize(vout.normal);
	float3 B = normalize(cross(T, N));
	// 3つのベクトルから、テクスチャの法線を
	// ワールド空間に変換するための行列を計算
	float3x3 tangentMat = float3x3(T, B, N);

	// 法線との計算を行う
	// 光源のベクトルをワールド空間からテクスチャ空間へ移動させる
	// ※接ベクトルの変換行列の逆行列を求めればよい
	tangentMat = transpose(tangentMat);
	float3 invLightDir = normalize(-lightDir.xyz);
	invLightDir = mul(invLightDir, tangentMat);
	vout.lightT = invLightDir;

	// 視線ベクトルもワールド空間からテクスチャ空間へ移動させる
	float4 camVec = worldPos - camPos;
	vout.camVecT = mul(camVec.xyz, tangentMat);

	return vout;
	*/

	VS_OUT vOut;
	vOut.pos = float4(vin.pos, 1.0f);
	vOut.pos = mul(vOut.pos, world);
	float4 worldPos = vOut.pos; // 計算途中でワールド座標の情報を変数にコピー
	vOut.worldPos = worldPos;
	vOut.pos = mul(vOut.pos, view);
	vOut.pos = mul(vOut.pos, proj);
	vOut.tex = vin.tex;
	vOut.normal = mul(vin.normal, (float3x3) world);

	vOut.tangentW = float4(mul(vin.tangent.xyz, (float3x3) world), 1.0);

	return vOut;
}