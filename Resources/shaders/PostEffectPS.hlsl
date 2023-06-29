#include "PostEffect.hlsli"

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

	//float4 maxColor = {1,1,1,0};
	
	//•W€F
	float4 texcolor1 = tex.Sample(smp, input.uv);

	//“ñŒÂ–Ú
	/*float4 texcolor2;
	
	float Ushift = 3.0f / 1280.0f;
	float Vshift = 3.0f / 720.0f;

	float4 SumColor = { 0,0,0,0 };

	SumColor += tex.Sample(smp, input.uv + float2(-Ushift, -Vshift));
	SumColor += tex.Sample(smp, input.uv + float2(0.0f   , -Vshift));
	SumColor += tex.Sample(smp, input.uv + float2(+Ushift, -Vshift));

	SumColor += tex.Sample(smp, input.uv + float2(-Ushift, 0.0f));
	SumColor += tex.Sample(smp, input.uv + float2(   0.0f, 0.0f));
	SumColor += tex.Sample(smp, input.uv + float2(+Ushift, 0.0f));

	SumColor += tex.Sample(smp, input.uv + float2(-Ushift, +Vshift));
	SumColor += tex.Sample(smp, input.uv + float2(   0.0f, +Vshift));
	SumColor += tex.Sample(smp, input.uv + float2(+Ushift, +Vshift));

	SumColor /= 9;*/
	
	//float4 color = SumColor;
	//float4 color = texcolor1;

	//F”½“]‘ã“ü
	//float4 color = maxColor - texcolor1;
	//float4 color = texcolor1;


	//‚µ‚Ü‚µ‚Üˆ—
	/*if (fmod(input.uv.y, 0.1f) < 0.05f) {
		color = SumColor;
	}*/

	return float4(texcolor1.rgb, 1);
	//return float4(color.rgb, 1);
}