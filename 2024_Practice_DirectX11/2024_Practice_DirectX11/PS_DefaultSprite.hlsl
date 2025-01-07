struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};

Texture2D myTexture : register(t0);
SamplerState mySampler : register(s0);


float4 main(PS_IN pin) : SV_TARGET
{
	return myTexture.Sample(mySampler, pin.uv) * pin.color;
}
