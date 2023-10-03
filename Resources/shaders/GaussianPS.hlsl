#include "Gaussian.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> tex2 : register(t1);

SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	//float4 haiColor = tex.Sample(smp, input.uv);

	//float grayScale = haiColor.r * 0.299f + haiColor.g * 0.587f + haiColor.b * 0.114f;
	//float extract = smoothstep(0.6,0.9,grayScale);
	//haiColor *= extract;

	//ガウシアンブラー
	float Ushift = 6.0f / 1280.0f;
	float Vshift = 6.0f / 720.0f;

	//標準色
	float4 drawUV = tex.Sample(smp, input.uv);

	float2 drawUV2 = input.uv;

	float totalWeight = 0;
	float sigma = 0.005f;
	float stepWidth = 0.002f;
	float4 color = { 0,0,0,0 };

	for (float py = -sigma * 2; py <= sigma * 2; py += stepWidth) {
		for (float px = -sigma * 2; px <= sigma * 2; px += stepWidth) {
			float2 pickUV = input.uv + float2(px, py);

			float d = distance(drawUV2, pickUV);
			d = exp(-(d * d) / (2 * sigma * sigma));

			float weight = d;

			color += tex.Sample(smp, pickUV) * weight;
			totalWeight += weight;
		}
	}

	color.rgb = color.rgb / totalWeight;

	return float4(color.rgb, 1);
	//return float4(haiColor.rgb, 1);
}