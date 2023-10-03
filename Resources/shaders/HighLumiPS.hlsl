#include "HighLumiShader.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> tex2 : register(t1);

SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 haiColor = tex.Sample(smp, input.uv);

	float grayScale = haiColor.r * 0.299f + haiColor.g * 0.587f + haiColor.b * 0.114f;
	float extract = smoothstep(0.6,0.9,grayScale);
	haiColor *= extract;

	return float4(haiColor.rgb, 1);
}