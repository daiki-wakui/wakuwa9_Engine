#include "PostEffect.h"
#include <d3dx12.h>

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
