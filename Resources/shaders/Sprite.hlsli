//�}�e���A��
cbuffer ConstBufferDataMaterial : register(b0)
{
	float4 color;	//�F(RGBA)
};

//���_�f�[�^�̏o�͍\����
struct VSOutput
{
	//�V�X�e���p���_���W
	float4 svpos : SV_POSITION;
	//uv�l
	float2 uv : TEXCOORD;
};