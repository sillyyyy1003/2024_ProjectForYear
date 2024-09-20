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
    float3 center;  // �g�̒��S�ʒu
    float amplitude;// �g�̕�
    float frequency;// �g�̕p�x
    float speed;    // �g�̃X�r�[�h
    float sigma;    // �g�̊g�U�͈�
    float time;     // �o�ߎ���
}



VS_OUT main(VS_IN vin)
{
    VS_OUT vOut;

    //�����Ŕg�ɂ�闱�q�̌v�Z���s��

    float distanceToCenter = length(vin.pos.xz - center.xz);
    
    if (distanceToCenter < sigma)
    {
        float waveHeight = amplitude * exp(-distanceToCenter * distanceToCenter / (2 * sigma * sigma)) * sin(2 * 3.14159 * (frequency * time - distanceToCenter /speed));

        vin.pos.y += waveHeight;
    }

    vOut.pos = float4(vin.pos, 1.0f);
    vOut.pos = mul(vOut.pos, world);
	// �s�N�Z���V�F�[�_�[�ɓn�����߂ɁA�r���ŕϐ��ɃR�s�[
    vOut.worldPos = vOut.pos;

    vOut.pos = mul(vOut.pos, view);
    vOut.pos = mul(vOut.pos, proj);

    vOut.normal = mul(vin.normal, (float3x3) world);
    vOut.tex = vin.tex;
    return vOut;
}