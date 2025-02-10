struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
	float2 tex : TEXCOORD;
};

Texture2D myTex : register(t0);
SamplerState mySampler : register(s0);


float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = myTex.Sample(mySampler, pin.tex);
	clip(color.a - 0.1f);

	return saturate(color * pin.color);
}