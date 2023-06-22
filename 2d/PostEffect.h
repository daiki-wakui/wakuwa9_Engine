#pragma once
#include "Sprite.h"
#include <cstdint>

class PostEffect :  public Sprite
{
private:

	ComPtr<ID3D12Resource> texBuff_;

	ComPtr<ID3D12DescriptorHeap> descHeapSRV_;


	//�[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuff_;

	//RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;

	//DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapDSV_;


	D3D12_RESOURCE_BARRIER barrierDesc_{};

	//�萔�o�b�t�@�̐���
	//color
	ComPtr<ID3D12Resource> constBuffMaterial_ = nullptr;
	//mat
	ComPtr<ID3D12Resource> constBuffTransform_ = nullptr;

	//�O���t�B�b�N�X�p�C�v���C��
	ComPtr<ID3D12PipelineState> pipelineState;

	//���[�g�V�O�l�N�`��
	ComPtr<ID3D12RootSignature> rootSignature;

	//�؂�Ă���
	SpriteBasis* spBasis_ = nullptr;
	WindowsApp* winApp_ = nullptr;


	static const float sCLEAR_COLOR[4];

public:

	void Initialize();

	void VertexData();

	void IndexData();

	void Crate();

	void SetDirectX(SpriteBasis* spBasis, WindowsApp* winApp);

	void CreateGraphicsPipelineState();

	//�R���X�g���N�^
	PostEffect();

	//�`��
	void Draw();


	//�V�[���`��O����
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

	//�V�[���`��㏈��
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

};

