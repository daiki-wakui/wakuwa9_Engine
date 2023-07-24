#pragma once
#include "SpriteBasis.h"
#include "WindowsApp.h"
#include <DirectXMath.h>
using namespace DirectX;

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
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	UINT vertexSize_;

	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle_;

	ComPtr<ID3D12Resource> indexBuff_ = nullptr;
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView_{};
	UINT indexSize_;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES heapProp_{};
	//リソース設定
	D3D12_RESOURCE_DESC resDesc_{};

	//定数バッファの生成
	//color
	ComPtr<ID3D12Resource> constBuffMaterial_ = nullptr;
	//mat
	ComPtr<ID3D12Resource> constBuffTransform_ = nullptr;

	//頂点データ構造体
	struct Vertex {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};

	//定数バッファ用データ構造体(マテリアル)
	struct ConstBufferDataMaterial {
		DirectX::XMFLOAT4 color;	//色(RGBA)
		float power;
	};

	//定数バッファ用データ構造体(3D変換行列)
	struct CounstBufferDataTransform {
		DirectX::XMMATRIX mat;	//3D変換行列
	};

	CounstBufferDataTransform* constMapTransform_ = nullptr;

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

public:

	const DirectX::XMFLOAT2& GetPosition() const { return position_; };
	void SetPosition(const DirectX::XMFLOAT2& position) { position_ = position; };

	const float GetRotation() const { return rotation_; };
	void SetRotation(const float rotation) { rotation_ = rotation; };

	const DirectX::XMFLOAT2& GetSize() const { return size_; };
	void SetSize(const DirectX::XMFLOAT2& size) { size_ = size; };

	const DirectX::XMFLOAT2& GetAncP() const { return anchorPoint_; };
	void SetAncP(const DirectX::XMFLOAT2& anchorPoint) { anchorPoint_ = anchorPoint; };

	void Create(float x, float y);

	void Initialize(SpriteBasis* spBasis, WindowsApp* winApp);
	void Update();
	void Draw(int32_t texNum);

	void VertexData();
	void IndexData();
};



