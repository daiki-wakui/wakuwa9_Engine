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
public:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	HRESULT result;

	WindowsApp* winApp = nullptr;
	SpriteBasis* spBasis;

	ComPtr<ID3D12Resource> vertBuff = nullptr;
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	UINT vertexSize;

	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;

	ComPtr<ID3D12Resource> indexBuff = nullptr;
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};
	UINT indexSize;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};

	//定数バッファの生成
	//color
	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
	//mat
	ComPtr<ID3D12Resource> constBuffTransform = nullptr;

	//頂点データ構造体
	struct Vertex {
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

	//定数バッファ用データ構造体(マテリアル)
	struct ConstBufferDataMaterial {
		XMFLOAT4 color;	//色(RGBA)
	};

	//定数バッファ用データ構造体(3D変換行列)
	struct CounstBufferDataTransform {
		XMMATRIX mat;	//3D変換行列
	};

	CounstBufferDataTransform* constMapTransform = nullptr;

	Vertex vertices[4];

	XMMATRIX matWorld;
	XMMATRIX matScale;
	XMMATRIX matRot;
	XMMATRIX matTrans;
	XMMATRIX matProjection;

	XMFLOAT2 position_ = { 0,0 };
	float rotation_ = 0;
	XMFLOAT2 size_ = { 100.0f,100.0f };
	XMFLOAT2 anchorPoint_ = { 0.5f,0.5f };

public:

	const XMFLOAT2& GetPosition() const { return position_; };
	void SetPosition(const XMFLOAT2& position) { position_ = position; };

	const float GetRotation() const { return rotation_; };
	void SetRotation(float rotation) { rotation_ = rotation; };

	const XMFLOAT2& GetSize() const { return size_; };
	void SetSize(const XMFLOAT2& size) { size_ = size; };

	const XMFLOAT2& GetAncP() const { return anchorPoint_; };
	void SetAncP(const XMFLOAT2& anchorPoint) { anchorPoint_ = anchorPoint; };

	void Create(float x, float y);

	void Initialize(SpriteBasis* spBasis, WindowsApp* winApp);
	void Update();
	void Draw(int32_t texNum);

	void VertexData();
	void IndexData();
};



