#pragma once
#include "SpriteBasis.h"
#include "WindowsApp.h"
#include <DirectXMath.h>
using namespace DirectX;

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

public:
	
	void Create(float x, float y);

	void Initialize(SpriteBasis* spBasis, WindowsApp* winApp);
	void Update();
	void Draw();

	void VertexData();
	void IndexData();
};

