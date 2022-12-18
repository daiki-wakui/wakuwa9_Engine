#include "Sprite.h"

void Sprite::Create()
{
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


	//�萔�o�b�t�@�̐���
	result = spBasis->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&cbHeapProp,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�̃}�b�s���O
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);	//�}�b�s���O
	assert(SUCCEEDED(result));

	//�l����������Ŏ����]��
	constMapMaterial->color = XMFLOAT4(1, 0, 0, 0.5f);	//�F�ύX
}

void Sprite::Initialize(SpriteBasis* spBasis)
{
	this->spBasis = spBasis;

	VertexData();
}

void Sprite::Update()
{

}

void Sprite::Draw()
{
	// �p�C�v���C���X�e�[�g�ݒ�
	spBasis->GetDxBasis()->GetCommandList()->SetPipelineState(spBasis->GetPipelineState());

	// ���[�g�V�O�l�`���̐ݒ�R�}���h
	spBasis->GetDxBasis()->GetCommandList()->SetGraphicsRootSignature(spBasis->GetRootSignature());

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	spBasis->GetDxBasis()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // �O�p�`���X�g

	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	spBasis->GetDxBasis()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	spBasis->GetDxBasis()->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

	// �`��R�}���h
	//dxBasis->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0); // �S�Ă̒��_���g���ĕ`��
	spBasis->GetDxBasis()->GetCommandList()->DrawInstanced(vertexSize, 1, 0, 0); // �S�Ă̒��_���g���ĕ`��
}

void Sprite::VertexData()
{
#pragma region  ���_�f�[�^

	// ���_�f�[�^
	XMFLOAT3 vertices[] = {
	{ -0.5f, -0.5f, 0.0f }, // ����
		{ -0.5f, -0.5f, 0.0f }, // ����
		{ +0.5f, -0.5f, 0.0f }, // �E��
		{ -0.5f,  0.0f, 0.0f }, // ����
		{ +0.5f,  0.0f, 0.0f }, // �E��
		{ -0.5f, +0.5f, 0.0f }, // ����
		{ +0.5f, +0.5f, 0.0f }, // �E��
	};
	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));

	vertexSize = _countof(vertices);

#pragma endregion

#pragma region  ���_�o�b�t�@�̐ݒ�
	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

#pragma endregion

#pragma region  ���_�o�b�t�@�̐���

	// ���_�o�b�t�@�̐���
	
	result = spBasis->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

#pragma endregion

#pragma region  ���_�o�b�t�@�ւ̃f�[�^�]��

	//GPU��̃o�b�t�@�ɑΉ��������z������
	XMFLOAT3* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//�S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i];
	}
	//�q���������
	vertBuff->Unmap(0, nullptr);

#pragma endregion

#pragma region  ���_�o�b�t�@�r���[�̐���

	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(XMFLOAT3);

#pragma endregion
}
