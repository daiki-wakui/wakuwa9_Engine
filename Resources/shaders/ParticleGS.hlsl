#include "Particle.hlsli"

static const uint vnum = 4;
static const float4 offset_arrey[vnum] = {
	float4(-0.5f,-0.5f,0,0),	//����
	float4(-0.5f,+0.5f,0,0),	//����
	float4(+0.5f,-0.5f,0,0),	//�E��
	float4(+0.5f,+0.5f,0,0)		//�E��
};

static const float2 uv_arrey[vnum] = {
	float2(0,1),	//����
	float2(0,0),	//����
	float2(1,1),	//�E��
	float2(1,0)		//�E��
};

[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;

	//4���_�܂킷
	for (uint i = 0; i < vnum; i++) {
		//���[���h���W�x�[�X�ŁA���炷
		float4 offset;
		//offset = mul(matBillboard, offset_arrey[i]);
		offset = offset_arrey[i] * input[0].scale;
		offset = mul(matBillboard, offset);

		element.svpos = input[0].pos + offset;
		//�r���[�A�ˉe�ϊ�
		element.svpos = mul(mat, element.svpos);
		//element.uv = float2(0.5f, 0.5f);
		element.uv = uv_arrey[i];
		output.Append(element);
	}

	/*element.normal = input[0].normal;
	element.uv = input[0].uv;
	element.svpos = input[0].svpos;
	output.Append(element);

	element.svpos = input[0].svpos + float4(10.0f, 10.0f, 0.0f, 0.0f);
	output.Append(element);

	element.svpos = input[0].svpos + float4(10.0f, 0.0f, 0.0f, 0.0f);
	output.Append(element);*/
	
	/*for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.svpos  = input[i].svpos;
		element.normal = input[i].normal;
		element.uv     = input[i].uv;
		output.Append(element);
	}
	output.RestartStrip();

	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.svpos = input[i].svpos + float4(20.0f, 0.0f, 0.0f, 0.0f);
		element.normal = input[i].normal;
		element.uv = input[i].uv * 5.0f;
		output.Append(element);
	}*/
}