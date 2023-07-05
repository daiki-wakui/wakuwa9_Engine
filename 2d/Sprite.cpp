#include "Sprite.h"
using namespace DirectX;

void Sprite::Create(float x, float y)
{

	//�q�[�v�ݒ�(color)
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

	//�q�[�v�ݒ�(mat)
	D3D12_HEAP_PROPERTIES cbMatHeapProp{};
	cbMatHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbMatResourceDesc{};
	cbMatResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbMatResourceDesc.Width = (sizeof(CounstBufferDataTransform) + 0xff) & ~0xff;
	cbMatResourceDesc.Height = 1;
	cbMatResourceDesc.DepthOrArraySize = 1;
	cbMatResourceDesc.MipLevels = 1;
	cbMatResourceDesc.SampleDesc.Count = 1;
	cbMatResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


	//color
	//�萔�o�b�t�@�̐���
	result_ = spBasis_->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&cbHeapProp,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial_));
	assert(SUCCEEDED(result_));

	//mat
	//�萔�o�b�t�@�̐���
	result_ = spBasis_->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&cbMatHeapProp,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbMatResourceDesc,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform_));
	assert(SUCCEEDED(result_));

	//�萔�o�b�t�@�̃}�b�s���O(color)
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result_ = constBuffMaterial_->Map(0, nullptr, (void**)&constMapMaterial);	//�}�b�s���O
	assert(SUCCEEDED(result_));

	//�萔�o�b�t�@�̃}�b�s���O(mat)
	result_ = constBuffTransform_->Map(0, nullptr, (void**)&constMapTransform_);
	assert(SUCCEEDED(result_));

	//�l����������Ŏ����]��
	constMapMaterial->color = XMFLOAT4(1, 1, 1, 1);	//�F�ύX

	//��������_�ɐݒ�
	matProjection_ =
		XMMatrixOrthographicOffCenterLH(
			0.0f, (float)winApp_->GetWindowWidth(),
			(float)winApp_->GetWindowHeight(), 0.0f,
			0.0f, 1.0f);

	matWorld_ = XMMatrixIdentity();

	matScale_ = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	matWorld_ *= matScale_;

	matRot_ = XMMatrixIdentity();
	matRot_ *= XMMatrixRotationZ(XMConvertToRadians(0.0f));	//Z 0�x��]
	matRot_ *= XMMatrixRotationX(XMConvertToRadians(0.0f));	//X 0�x��]
	matRot_ *= XMMatrixRotationY(XMConvertToRadians(0.0f));	//Y 0�x��]
	matWorld_ *= matRot_;

	matTrans_ = XMMatrixTranslation(x, y, 0.0f);
	matWorld_ *= matTrans_;

	constMapTransform_->mat = matWorld_ * matProjection_;

	XMFLOAT2 pos = { 0,0 };

}

void Sprite::Initialize(SpriteBasis* spBasis, WindowsApp* winApp)
{
	this->spBasis_ = spBasis;
	this->winApp_ = winApp;

	VertexData();
	IndexData();
}

void Sprite::Update()
{
	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;

	vertices_[LB].pos = { left ,bottom,0.0f };
	vertices_[LT].pos = { left ,top   ,0.0f };
	vertices_[RB].pos = { right,bottom,0.0f };
	vertices_[RT].pos = { right,top   ,0.0f };

	//GPU��̃o�b�t�@�ɑΉ��������z������
	Vertex* vertMap = nullptr;
	result_ = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result_));
	//�S���_�ɑ΂���
	for (int32_t i = 0; i < _countof(vertices_); i++) {
		vertMap[i] = vertices_[i];
	}
	//�q���������
	vertBuff_->Unmap(0, nullptr);


	matRot_ = XMMatrixIdentity();
	matRot_ *= XMMatrixRotationZ(XMConvertToRadians(rotation_));	//Z 0�x��]
	matRot_ *= XMMatrixRotationX(XMConvertToRadians(0));	//X 0�x��]
	matRot_ *= XMMatrixRotationY(XMConvertToRadians(0));	//Y 0�x��]
	matWorld_ *= matRot_;

	matTrans_ = XMMatrixTranslation(position_.x, position_.y, 0.0f);
	matWorld_ *= matTrans_;

	constMapTransform_->mat = matWorld_ * matProjection_;

}

void Sprite::Draw(int32_t texNum)
{
	// �p�C�v���C���X�e�[�g�ݒ�
	spBasis_->GetDxBasis()->GetCommandList()->SetPipelineState(spBasis_->GetPipelineState());

	// ���[�g�V�O�l�`���̐ݒ�R�}���h
	spBasis_->GetDxBasis()->GetCommandList()->SetGraphicsRootSignature(spBasis_->GetRootSignature());

	spBasis_->GetDxBasis()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g

	// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	spBasis_->GetDxBasis()->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial_->GetGPUVirtualAddress());

	//�f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { spBasis_->srvHeap_.Get() };
	spBasis_->GetDxBasis()->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//SRV�q�[�v�̐擪�A�h���X���擾
	srvGpuHandle_ = spBasis_->srvHeap_->GetGPUDescriptorHandleForHeapStart();
	srvGpuHandle_.ptr += spBasis_->GetincrementSize() * texNum;

	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
	spBasis_->GetDxBasis()->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle_);

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	spBasis_->GetDxBasis()->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform_->GetGPUVirtualAddress());

	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	spBasis_->GetDxBasis()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);

	// �C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	spBasis_->GetDxBasis()->GetCommandList()->IASetIndexBuffer(&ibView_);

	// �`��R�}���h
	//dxBasis->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0); // �S�Ă̒��_���g���ĕ`��
	spBasis_->GetDxBasis()->GetCommandList()->DrawIndexedInstanced(indexSize_, 1, 0, 0, 0); // �S�Ă̒��_���g���ĕ`��
}

void Sprite::VertexData()
{
#pragma region  ���_�f�[�^

	// ���_�f�[�^
	vertices_[LB] = { {   0.0f, 100.0f, 0.0f },{ 0.0f , 1.0f } };
	vertices_[LT] = { {   0.0f, 0.0f, 0.0f }, { 0.0f , 0.0f } };
	vertices_[RB] = { { 100.0f, 100.0f, 0.0f },{ 1.0f , 1.0f } };
	vertices_[RT] = { { 100.0f,   0.0f, 0.0f },{ 1.0f , 0.0f } };

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * _countof(vertices_));

	vertexSize_ = _countof(vertices_);

#pragma endregion

#pragma region  ���_�o�b�t�@�̐ݒ�
	// ���_�o�b�t�@�̐ݒ�
	heapProp_.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
	// ���\�[�X�ݒ�
	resDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc_.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc_.Height = 1;
	resDesc_.DepthOrArraySize = 1;
	resDesc_.MipLevels = 1;
	resDesc_.SampleDesc.Count = 1;
	resDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

#pragma endregion

#pragma region  ���_�o�b�t�@�̐���

	// ���_�o�b�t�@�̐���

	result_ = spBasis_->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&heapProp_, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc_, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result_));

#pragma endregion

#pragma region  ���_�o�b�t�@�ւ̃f�[�^�]��

	//GPU��̃o�b�t�@�ɑΉ��������z������
	Vertex* vertMap = nullptr;
	result_ = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result_));
	//�S���_�ɑ΂���
	for (int32_t i = 0; i < _countof(vertices_); i++) {
		vertMap[i] = vertices_[i];
	}
	//�q���������
	vertBuff_->Unmap(0, nullptr);

#pragma endregion

#pragma region  ���_�o�b�t�@�r���[�̐���

	// GPU���z�A�h���X
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView_.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbView_.StrideInBytes = sizeof(vertices_[0]);

#pragma endregion
}

void Sprite::IndexData()
{
	//�C���f�b�N�X�f�[�^
	uint16_t indices[] = {
		0 , 1 , 2,	//�O�p�`1��
		1 , 2 , 3,	//�O�p�`2��
	};

	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	indexSize_ = _countof(indices);

	//���\�[�X�ݒ�
	resDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc_.Width = sizeIB;	//�C���f�b�N�X��񂪓��镪�̃T�C�Y
	resDesc_.Height = 1;
	resDesc_.DepthOrArraySize = 1;
	resDesc_.MipLevels = 1;
	resDesc_.SampleDesc.Count = 1;
	resDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	heapProp_.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

	//�C���f�b�N�X�o�b�t�@�̐���
	result_ = spBasis_->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&heapProp_,
		D3D12_HEAP_FLAG_NONE,
		&resDesc_,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff_));

	//�C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result_ = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	//�S�C���f�b�N�X�ɑ΂���
	for (int32_t i = 0; i < _countof(indices); i++) {
		indexMap[i] = indices[i];
	}
	//�}�b�s���O����
	indexBuff_->Unmap(0, nullptr);

	//�C���f�b�N�X�o�b�t�@�r���[�̐���
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	ibView_.SizeInBytes = sizeIB;
}

