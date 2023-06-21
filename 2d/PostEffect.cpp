#include "PostEffect.h"
#include <d3dx12.h>
#include <cassert>

void PostEffect::Initialize()
{
	HRESULT result;

	Sprite::Initialize(spBasis_, winApp_);


	//�e�N�X�`���o�b�t�@�ݒ�
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//�e�N�X�`�����\�[�X�ݒ�
	D3D12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		winApp_->GetWindowWidth(),
		(UINT)winApp_->GetWindowHeight(),
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	//�e�N�X�`���o�b�t�@�̐���
	result = spBasis_->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texBuff_));

	assert(SUCCEEDED(result));


	//�C���[�W�f�[�^�]��
	const UINT pixelCount = winApp_->GetWindowWidth() * winApp_->GetWindowHeight();

	const UINT rowPitch = sizeof(UINT) * winApp_->GetWindowHeight();

	const UINT deptPitch = rowPitch * winApp_->GetWindowHeight();

	UINT* img = new UINT[pixelCount];

	for (UINT i = 0; i < pixelCount; i++) {
		img[i] = 0xff0000ff;
	}

	result = texBuff_->WriteToSubresource(0, nullptr, img, rowPitch, deptPitch);
	assert(SUCCEEDED(result));
	delete[] img;

	//SRV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;

	//SRV�p�f�X�N���v�^�q�[�v����
	result = spBasis_->GetDxBasis()->GetDevice()->CreateDescriptorHeap(
		&srvDescHeapDesc,
		IID_PPV_ARGS(&descHeapSRV_));

	assert(SUCCEEDED(result));


	//SRV�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//�ݒ�\����
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	//�f�X�N���v�^�q�[�v��SRV�쐬
	spBasis_->GetDxBasis()->GetDevice()->CreateShaderResourceView(
		texBuff_.Get(),
		&srvDesc,
		descHeapSRV_->GetCPUDescriptorHandleForHeapStart()
	);
}

void PostEffect::SetDirectX(SpriteBasis* spBasis, WindowsApp* winApp)
{
	spBasis_ = spBasis;
	winApp_ = winApp;
}

PostEffect::PostEffect()
{

}

void PostEffect::Draw(int32_t texNum)
{
	// �p�C�v���C���X�e�[�g�ݒ�
	spBasis_->GetDxBasis()->GetCommandList()->SetPipelineState(spBasis_->GetPipelineState());

	// ���[�g�V�O�l�`���̐ݒ�R�}���h
	spBasis_->GetDxBasis()->GetCommandList()->SetGraphicsRootSignature(spBasis_->GetRootSignature());

	//�v���j�e�B�u�`���ݒ�
	spBasis_->GetDxBasis()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // �O�p�`���X�g

	// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	spBasis_->GetDxBasis()->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial_->GetGPUVirtualAddress());

	//�f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV_.Get() };
	spBasis_->GetDxBasis()->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//SRV�q�[�v�̐擪�A�h���X���擾
	/*srvGpuHandle_ = spBasis_->srvHeap_->GetGPUDescriptorHandleForHeapStart();
	srvGpuHandle_.ptr += spBasis_->GetincrementSize() * texNum;*/

	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
	spBasis_->GetDxBasis()->GetCommandList()->SetGraphicsRootDescriptorTable(1, descHeapSRV_->GetGPUDescriptorHandleForHeapStart());

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
