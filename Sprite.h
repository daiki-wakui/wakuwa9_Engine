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
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	UINT vertexSize;

	//定数バッファの生成
	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;

	//定数バッファ用データ構造体(マテリアル)
	struct ConstBufferDataMaterial {
		XMFLOAT4 color;	//色(RGBA)
	};

public:
	
	void Create();

	void Initialize(SpriteBasis* spBasis);
	void Update();
	void Draw();

	void VertexData();
};

