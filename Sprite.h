#pragma once
#include "SpriteBasis.h"
#include <DirectXMath.h>
using namespace DirectX;

class Sprite
{
public:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	HRESULT result;

	SpriteBasis* spBasis;

	ComPtr<ID3D12Resource> vertBuff = nullptr;
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	UINT vertexSize;


	ComPtr<ID3D12Resource> indexBuff = nullptr;
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};
	UINT indexSize;

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};

	//�萔�o�b�t�@�̐���
	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;

	//���_�f�[�^�\����
	struct Vertex {
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

	//�萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
	struct ConstBufferDataMaterial {
		XMFLOAT4 color;	//�F(RGBA)
	};

public:
	
	void Create();

	void Initialize(SpriteBasis* spBasis);
	void Update();
	void Draw();

	void VertexData();
	void IndexData();
};

