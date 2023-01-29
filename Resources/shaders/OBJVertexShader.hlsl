#include "OBJShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{

	/*float3 lightdir = float3(1, -1, 1);
	lightdir = normalize(lightdir);

	float3 lightcolor = float3(1, 1, 1);*/

	//�@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
	float4 wnormal = normalize(mul(world, float4(normal, 0)));
	float4 wpos = mul(world, pos);

	//�����ˌ�
	float3 ambient = m_ambient;

	//�g�U���ˌ�
	float3 diffuse = dot(lightv, wnormal.xyz) * m_diffuse;

	//���ʔ��ˌ�
	//���_���W
	//const float3 eye = float3(0, 0, -20);
	//����x
	const float3 shininess = 4.0f;
	//���_���王�_�ւ̕����x�N�g��
	//float3 eyedir = normalize(eye - pos.xyz);
	float3 eyedir = normalize(cameraPos - wpos.xyz);

	//���ˌ��x�N�g��
	float3 reflect = normalize(-lightv + 2 * dot(lightv, wnormal.xyz) * wnormal.xyz);


	//���ʔ��ˌ�
	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;


	//�s�N�Z���V�F�[�_�ɓn���l
	VSOutput output;
	output.svpos = mul(mul(viewproj, world), pos);

	//�S�ĉ��Z
	output.color.rgb = (ambient + diffuse + specular) * lightcolor;

	//output.color.rgb = dot(-lightdir, normal) * m_diffuse * lightcolor;
	output.color.a = m_alpha;
	output.uv = uv;
	return output;
}