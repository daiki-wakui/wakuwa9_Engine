#include "Gaussian.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> tex2 : register(t1);

SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	/*float4 texcolor1 = tex.Sample(smp,input.uv);
	float4 texcolor2 = tex2.Sample(smp, input.uv);

	float4 color = texcolor1;
	if (fmod(input.uv.y, 0.1f) < 0.05f) {
		color = texcolor2;
	}*/

	float4 maxColor = {1,1,1,0};

	//�W���F
	float4 texcolor1 = tex.Sample(smp, input.uv);

	float4 color = texcolor1;

	//return float4(maxColor.rgb - color.rgb, 1);
	return float4(color.rgb, 1);
}