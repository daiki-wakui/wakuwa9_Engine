#include "Sprite.h"

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
	result = spBasis->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&cbHeapProp,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result));

	//mat
	//�萔�o�b�t�@�̐���
	result = spBasis->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&cbMatHeapProp,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbMatResourceDesc,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform));
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�̃}�b�s���O(color)
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);	//�}�b�s���O
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�̃}�b�s���O(mat)
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));

	//�l����������Ŏ����]��
	constMapMaterial->color = XMFLOAT4(1, 1, 1, 1);	//�F�ύX

	//��������_�ɐݒ�
	matProjection =
		XMMatrixOrthographicOffCenterLH(
			0.0f, winApp->GetWindowWidth(),
			(float)winApp->GetWindowHeight(), 0.0f,
			0.0f, 1.0f);

	matWorld = XMMatrixIdentity();

	matScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	matWorld *= matScale;

	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(0.0f));	//Z 0�x��]
	matRot *= XMMatrixRotationX(XMConvertToRadians(0.0f));	//X 0�x��]
	matRot *= XMMatrixRotationY(XMConvertToRadians(0.0f));	//Y 0�x��]
	matWorld *= matRot;

	matTrans = XMMatrixTranslation(x, y, 0.0f);
	matWorld *= matTrans;

	constMapTransform->mat = matWorld * matProjection;

	XMFLOAT2 pos = { 0,0 };

}

void Sprite::Initialize(SpriteBasis* spBasis, WindowsApp* winApp)
{
	this->spBasis = spBasis;
	this->winApp = winApp;

	VertexData();
	IndexData();
}

void Sprite::Update()
{
	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;

	vertices[LB].pos = { left ,bottom,0.0f };
	vertices[LT].pos = { left ,top   ,0.0f };
	vertices[RB].pos = { right,bottom,0.0f };
	vertices[RT].pos = { right,top   ,0.0f };

	//GPU��̃o�b�t�@�ɑΉ��������z������
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//�S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i];
	}
	//�q���������
	vertBuff->Unmap(0, nullptr);


	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation_));	//Z 0�x��]
	matRot *= XMMatrixRotationX(XMConvertToRadians(0));	//X 0�x��]
	matRot *= XMMatrixRotationY(XMConvertToRadians(0));	//Y 0�x��]
	matWorld *= matRot;

	matTrans = XMMatrixTranslation(position_.x, position_.y, 0.0f);
	matWorld *= matTrans;

	constMapTransform->mat = matWorld * matProjection;

}

void Sprite::Draw(int texNum)
{
	// �p�C�v���C���X�e�[�g�ݒ�
	spBasis->GetDxBasis()->GetCommandList()->SetPipelineState(spBasis->GetPipelineState());

	// ���[�g�V�O�l�`���̐ݒ�R�}���h
	spBasis->GetDxBasis()->GetCommandList()->SetGraphicsRootSignature(spBasis->GetRootSignature());

	spBasis->GetDxBasis()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g

	// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	spBasis->GetDxBasis()->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

	//�f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { spBasis->srvHeap.Get() };
	spBasis->GetDxBasis()->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//SRV�q�[�v�̐擪�A�h���X���擾
	srvGpuHandle = spBasis->srvHeap->GetGPUDescriptorHandleForHeapStart();
	srvGpuHandle.ptr += spBasis->GetincrementSize() * texNum;

	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
	spBasis->GetDxBasis()->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	spBasis->GetDxBasis()->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	spBasis->GetDxBasis()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

	// �C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	spBasis->GetDxBasis()->GetCommandList()->IASetIndexBuffer(&ibView);

	// �`��R�}���h
	//dxBasis->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0); // �S�Ă̒��_���g���ĕ`��
	spBasis->GetDxBasis()->GetCommandList()->DrawIndexedInstanced(indexSize, 1, 0, 0, 0); // �S�Ă̒��_���g���ĕ`��
}

void Sprite::VertexData()
{
#pragma region  ���_�f�[�^

	// ���_�f�[�^
	vertices[LB] = { {   0.0f, 100.0f, 0.0f },{ 0.0f , 1.0f } };
	vertices[LT] = { {   0.0f, 0.0f, 0.0f }, { 0.0f , 0.0f } };
	vertices[RB] = { { 100.0f, 100.0f, 0.0f },{ 1.0f , 1.0f } };
	vertices[RT] = { { 100.0f,   0.0f, 0.0f },{ 1.0f , 0.0f } };

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

	vertexSize = _countof(vertices);

#pragma endregion

#pragma region  ���_�o�b�t�@�̐ݒ�
	// ���_�o�b�t�@�̐ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
	// ���\�[�X�ݒ�
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
	Vertex* vertMap = nullptr;
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
	vbView.StrideInBytes = sizeof(vertices[0]);

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

	indexSize = _countof(indices);

	//���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;	//�C���f�b�N�X��񂪓��镪�̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

	//�C���f�b�N�X�o�b�t�@�̐���
	result = spBasis->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	//�C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//�S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < _countof(indices); i++) {
		indexMap[i] = indices[i];
	}
	//�}�b�s���O����
	indexBuff->Unmap(0, nullptr);

	//�C���f�b�N�X�o�b�t�@�r���[�̐���
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
}

