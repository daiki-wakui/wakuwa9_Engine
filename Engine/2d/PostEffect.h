#pragma once
#include "Sprite.h"
#include "KeyBoard.h"
#include "Player.h"

#include <cstdint>
#include <random>

class PostEffect :  public Sprite
{
private:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:

	ComPtr<ID3D12Resource> texBuff_[2] = {};

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
	KeyBoard* key_ = nullptr;

	static const float sCLEAR_COLOR[4];

	ConstBufferDataMaterial* constMapMaterial = nullptr;

	float noizpow;

	int32_t frame_ = 0;
	int32_t noiseTimer_ = 0;

	bool isEffect_ = false;

public:

	void SetIsEffect(bool isEffect) { isEffect_ = isEffect; }

	void Initialize(int32_t num = 0);

	void VertexData();

	void IndexData();

	void Crate();

	void SetDirectX(SpriteBasis* spBasis, WindowsApp* winApp, KeyBoard* key);

	void CreateGraphicsPipelineState(int32_t num = 0);

	//�R���X�g���N�^
	PostEffect();

	void Update(Player* player);

	//�`��
	void Draw();


	//�V�[���`��O����
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

	//�V�[���`��㏈��
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

};
