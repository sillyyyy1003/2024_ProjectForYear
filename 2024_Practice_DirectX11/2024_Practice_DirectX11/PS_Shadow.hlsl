struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
	float4 screenPos : POSITION0;
};

SamplerState mySampler : register(s0);
Texture2D depthTex : register(t0);

float4 main(PS_IN pin):SV_TARGET
{
	float4 color = float4(0, 0, 0, 0);

	//Inverse the UV
	float2 screenUV = pin.screenPos.xy / pin.screenPos.w;
	screenUV = screenUV * 0.5f + 0.5f;
	screenUV.y = 1.0 - screenUV.y;

	float objDepth = pin.screenPos.z / pin.screenPos.w;
	float texDepth = depthTex.Sample(mySampler, screenUV).r;
	//if UV is Outside the screen
	if (screenUV.x < 0.f || screenUV.x > 1.f || screenUV.y < 0.0f || screenUV.y > 1.0f)
		texDepth = 1.f;
	
	clip(objDepth - texDepth);
	return float4(0.f, 0.f, 0.f, 0.6f); // ¸KêF

}
