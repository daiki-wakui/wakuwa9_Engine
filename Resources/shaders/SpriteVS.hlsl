#include "Basic.hlsli"

VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	VSOutput output;	//�s�N�Z���V�F�[�_�ɐF��n��
	output.svpos = pos;
	output.uv = uv;

	return output;
}