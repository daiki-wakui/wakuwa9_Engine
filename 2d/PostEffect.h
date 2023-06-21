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

	//�؂�Ă���
	SpriteBasis* spBasis_ = nullptr;
	WindowsApp* winApp_ = nullptr;


	static const float sCLEAR_COLOR[4];

public:

	void Initialize();

	void SetDirectX(SpriteBasis* spBasis, WindowsApp* winApp);

	//�R���X�g���N�^
	PostEffect();

	//�`��
	void Draw(int32_t texNum);


	//�V�[���`��O����
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

	//�V�[���`��㏈��
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

};

