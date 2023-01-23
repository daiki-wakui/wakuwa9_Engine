#include "OBJShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{

	float3 lightdir = float3(1, -1, 1);
	lightdir = normalize(lightdir);

	float3 lightcolor = float3(1, 1, 1);

	//�����ˌ�
	float3 ambient = m_ambient;

	//�g�U���ˌ�
	float3 diffuse = dot(-lightdir, normal) * m_diffuse;

	//���ʔ��ˌ�
	//���_���W
	const float3 eye = float3(0, 0, -20);
	//����x
	const float3 shininess = 4.0f;
	//���_���王�_�ւ̕����x�N�g��
	float3 eyedir = normalize(eye - pos.xyz);
	//���ˌ��x�N�g��
	float3 reflect = normalize(lightdir + 2 * dot(-lightdir, normal) * normal);
	//���ʔ��ˌ�
	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;


	//�s�N�Z���V�F�[�_�ɓn���l
	VSOutput output;
	output.svpos = mul(mat, pos);

	//�S�ĉ��Z
	output.color.rgb = (ambient + diffuse + specular) * lightcolor;

	//output.color.rgb = dot(-lightdir, normal) * m_diffuse * lightcolor;
	output.color.a = m_alpha;
	output.uv = uv;
	return output;
}