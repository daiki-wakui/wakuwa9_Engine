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


	//深度バッファ
	ComPtr<ID3D12Resource> depthBuff_;

	//RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;

	//DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV_;


	D3D12_RESOURCE_BARRIER barrierDesc_{};

	//定数バッファの生成
	//color
	ComPtr<ID3D12Resource> constBuffMaterial_ = nullptr;
	//mat
	ComPtr<ID3D12Resource> constBuffTransform_ = nullptr;

	//グラフィックスパイプライン
	ComPtr<ID3D12PipelineState> pipelineState;

	//ルートシグネクチャ
	ComPtr<ID3D12RootSignature> rootSignature;

	//借りてくる
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

	//コンストラクタ
	PostEffect();

	void Update(Player* player = nullptr);

	//描画
	void Draw();


	//シーン描画前処理
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

	//シーン描画後処理
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

};

