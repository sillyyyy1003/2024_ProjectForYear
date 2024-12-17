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
    float4 worldPos : POSITION0;
};

cbuffer WVP : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
}

cbuffer UVMatrix : register(b1)
{
	float4x4 uv;
	int isUseUV;
	int dum1, dum2, dum3;
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

	if (isUseUV == 0)
	{
		vOut.tex = vin.tex;
	}
	else
	{
		float4 tex4;
		tex4.xy = vin.tex;
		tex4.z = 0;
		tex4.w = 1;
		vOut.tex = mul(tex4, uv).xy;
	}

    return vOut;
}