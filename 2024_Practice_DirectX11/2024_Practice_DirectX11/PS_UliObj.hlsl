struct PS_IN
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR;
};

float4 main(PS_IN pin):SV_TARGET{

    float4 color;
    color = pin.color;
    return color;

}