cbuffer ConstBufferDataMaterial : register(b0)
{
	float4 color;	//�F
}

float4 main() : SV_TARGET
{
	return color;
}