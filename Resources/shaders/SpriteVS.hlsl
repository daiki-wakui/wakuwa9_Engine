#include "Basic.hlsli"

VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	VSOutput output;	//ピクセルシェーダに色を渡す
	output.svpos = pos;
	output.uv = uv;

	return output;
}