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

cbuffer WaterPara : register(b1)
{
    float3 center;  // 波の中心位置
    float amplitude;// 波の幅
    float frequency;// 波の頻度
    float speed;    // 波のスビード
    float sigma;    // 波の拡散範囲
    float time;     // 経過時間
}



VS_OUT main(VS_IN vin)
{
    VS_OUT vOut;

    //ここで波による粒子の計算を行う

    float distanceToCenter = length(vin.pos.xz - center.xz);
    
    if (distanceToCenter < sigma)
    {
        float waveHeight = amplitude * exp(-distanceToCenter * distanceToCenter / (2 * sigma * sigma)) * sin(2 * 3.14159 * (frequency * time - distanceToCenter /speed));

        vin.pos.y += waveHeight;
    }

    vOut.pos = float4(vin.pos, 1.0f);
    vOut.pos = mul(vOut.pos, world);
	// ピクセルシェーダーに渡すために、途中で変数にコピー
    vOut.worldPos = vOut.pos;

    vOut.pos = mul(vOut.pos, view);
    vOut.pos = mul(vOut.pos, proj);

    vOut.normal = mul(vin.normal, (float3x3) world);
    vOut.tex = vin.tex;
    return vOut;
}