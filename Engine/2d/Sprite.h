#pragma once
#include "SpriteBasis.h"
#include "WindowsApp.h"

#include <DirectXMath.h>
#include <cstdint>

enum VertexNumber
{
	LB,
	LT,
	RB,
	RT,
};

class Sprite
{
private:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

protected:

	HRESULT result_;

	WindowsApp* winApp_ = nullptr;
	SpriteBasis* spBasis_;

	ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	UINT vertexSize_;

	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle_;

	ComPtr<ID3D12Resource> indexBuff_ = nullptr;
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView_{};
	UINT indexSize_;

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp_{};
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc_{};

	//�萔�o�b�t�@�̐���
	//color
	ComPtr<ID3D12Resource> constBuffMaterial_ = nullptr;
	//mat
	ComPtr<ID3D12Resource> constBuffTransform_ = nullptr;

	//���_�f�[�^�\����
	struct Vertex {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};

	//�萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
	struct ConstBufferDataMaterial {
		DirectX::XMFLOAT4 color;	//�F(RGBA)
		float power;
		float shiftPower;
	};

	//�萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��)
	struct CounstBufferDataTransform {
		DirectX::XMMATRIX mat;	//3D�ϊ��s��
	};

	CounstBufferDataTransform* constMapTransform_ = nullptr;
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	Vertex vertices_[4];

	DirectX::XMMATRIX matWorld_;
	DirectX::XMMATRIX matScale_;
	DirectX::XMMATRIX matRot_;
	DirectX::XMMATRIX matTrans_;
	DirectX::XMMATRIX matProjection_;

	DirectX::XMFLOAT2 position_ = { 0,0 };
	float rotation_ = 0;
	DirectX::XMFLOAT2 size_ = { 100.0f,100.0f };
	DirectX::XMFLOAT2 anchorPoint_ = { 0.5f,0.5f };
	DirectX::XMFLOAT4 color_ = { 1,1,1,1 };

	DirectX::XMFLOAT2 textureLeftTop_ = { 0,0 };
	DirectX::XMFLOAT2 textureSize_ = { 128.0f,128.0f };

	//���E���]
	bool isFilpX_ = false;
	//�㉺���]
	bool isFilpY_ = false;

	int32_t index_;

public:	//�����o�֐�

	void Create(float x = 0, float y = 0);

	void Initialize(SpriteBasis* spBasis, WindowsApp* winApp, int32_t texIndex = UINT32_MAX);
	void Update();
	void Draw(int32_t texNum);

	void MatUpdate();

	void VertexData();
	void IndexData();
	void VertexDataTransfer();

	void RectTexture(const DirectX::XMFLOAT2& rectSize);

public:	//getter setter

	const DirectX::XMFLOAT2& GetPosition() const { return position_; };
	const float GetRotation() const { return rotation_; };
	const DirectX::XMFLOAT4& GetColor() const { return color_; };
	const DirectX::XMFLOAT2& GetSize() const { return size_; };
	const DirectX::XMFLOAT2& GetAncP() const { return anchorPoint_; };
	const bool GetFilpX() const { return isFilpX_; };
	const bool GetFilpY() const { return isFilpY_; };
	const DirectX::XMFLOAT2& GetTextureLeftTop_() const { return textureLeftTop_; };
	const DirectX::XMFLOAT2& GetTextureSize_() const { return textureSize_; };

	void SetPosition(const DirectX::XMFLOAT2& position) { position_ = position; };
	void SetRotation(const float rotation) { rotation_ = rotation; };
	void SetColor(const DirectX::XMFLOAT4& size) { color_ = size; };
	void SetSize(const DirectX::XMFLOAT2& size) { size_ = size; };
	void SetAncP(const DirectX::XMFLOAT2& anchorPoint) { anchorPoint_ = anchorPoint; };
	void SetFilpX(const bool isFilpX) { isFilpX_ = isFilpX; };
	void SetFilpY(const bool isFilpY) { isFilpY_ = isFilpY; };
	void SetTextureLeftTop_(const DirectX::XMFLOAT2& textureLeftTop) { textureLeftTop_ = textureLeftTop; };
	void SetTextureSize_(const DirectX::XMFLOAT2& textureSize) { textureSize_ = textureSize; };
};



