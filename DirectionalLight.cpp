#include "DirectionalLight.h"

using namespace DirectX;

//�ÓI�����o�ϐ��̎���
ID3D12Device* DirectionalLight::device = nullptr;

void DirectionalLight::StaticInitalize(ID3D12Device* device)
{
	//�ď������`�F�b�N
	assert(!DirectionalLight::device);
	//nullptr�`�F�b�N
	assert(device);
	//�ÓI�����o�ϐ��̃Z�b�g
	DirectionalLight::device = device;
}

DirectionalLight* DirectionalLight::Creare()
{
	//3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	DirectionalLight* instance = new DirectionalLight();
	//������
	instance->Initalize();
	//���������C���X�^���X��Ԃ�
	return instance;
}

void DirectionalLight::Initalize()
{
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
	HRESULT result;

	result = device->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));

	assert(SUCCEEDED(result));

	//result = device->CreateCommittedResource(
	//	&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
	//	D3D12_HEAP_FLAG_NONE,
	//	&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&constBuff)
	//);

	//if (FAILED(result)) { assert(0); }

	//�萔�o�b�t�@�փf�[�^�]��
	TransferConstBuffer();
}

void DirectionalLight::Update()
{
	//�l�̍X�V���������Ƃ������萔�o�b�t�@�ɓ]������
	if (dirty == true) {
		TransferConstBuffer();
		dirty = false;
	}
}

void DirectionalLight::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex)
{
	//�萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex,
		constBuff->GetGPUVirtualAddress());
}

void DirectionalLight::TransferConstBuffer()
{
	HRESULT result;
	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->lightv = -lightdir;
		constMap->lightcolor = lightcolor;
		constBuff->Unmap(0, nullptr);
	}
}

void DirectionalLight::SetLightDir(const XMVECTOR& lightdir)
{
	//���K�����ăZ�b�g
	this->lightdir = XMVector3Normalize(lightdir);
	dirty = true;
}

void DirectionalLight::SetLightColor(const XMFLOAT3& lightcolor)
{
	this->lightcolor = lightcolor;
	dirty = true;
}
