#pragma once
#include "FbxModel.h"
#include "FbxLoader.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>
#include <cstdint>

class FbxObject3d
{
protected:	//エイリアス

	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス

	// 定数バッファ用データ構造体
	struct ConstBufferDataTransform
	{
		//XMMATRIX mat;	// ３Ｄ変換行列

		XMMATRIX viewproj;	//ビュープロジェクション行列
		XMMATRIX world;		//ワールド行列
		XMFLOAT3 cameraPos;	//カメラ座標(ワールド座標)
	};

	//ボーンの最大数
	static const int32_t MAX_BONES = 32;

	//定数バッファ用データ構造体(スキニング)
	struct CounstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

public: //メンバ関数

	//初期化
	void Initialize();

	//毎フレーム
	void Update();

	void Draw();

	//モデルのセット
	void SetModel(FbxModel* model) { model_ = model; }

	static void PreSet(ID3D12GraphicsCommandList* cmdList);

	void PlayAnimation();

protected: //メンバ変数

	//定数バッファ
	ComPtr<ID3D12Resource> constBuffTransfrom_;

	//定数バッファ(スキン)
	ComPtr<ID3D12Resource> constBuffSkin_;

	XMFLOAT3 scale_ = { 1,1,1 };

	XMFLOAT3 rotation_ = { 0,0,0 };

	XMFLOAT3 positon_ = { 0,0,0 };

	XMMATRIX matWorld_ = {};

	FbxModel* model_ = nullptr;

	//一フレームの時間
	FbxTime frameTime_;

	FbxTime startTime_;

	FbxTime endTime_;

	FbxTime currentTime_;

	bool isPlay_ = false;

public:	//静的メンバ関数

	//setter
	static void SetDevice(ID3D12Device* device) { FbxObject3d::sDevice = device; }

	static void SetCamera(XMFLOAT3& eye, XMFLOAT3& target, XMFLOAT3& up) {
		FbxObject3d::sEye = eye,
		FbxObject3d::sTarget = target,
		FbxObject3d::sUp = up;
	}

	//グラフィックスパイプライン
	static void CreateGraphicsPipeline();

	//ルートシグネクチャ
	static ComPtr<ID3D12RootSignature> sRootsignature;

	//パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> sPipelinestate;

	// コマンドリスト
	static ID3D12GraphicsCommandList* sCmdList;

	static void InitializeCamera(int32_t window_width, int32_t window_height);

	static void StaticInitialize(ID3D12Device* device, int32_t window_width, int32_t window_height);


	void SetPosition(const XMFLOAT3& position) { positon_ = position; }
	void SetRotation(const XMFLOAT3& rotation) { rotation_ = rotation; }
	void SetScale(const XMFLOAT3& scale) { scale_ = scale; }


private:	//静的メンバ変数

	static ID3D12Device* sDevice;

	// ビュー行列
	static XMMATRIX sMatView;
	// 射影行列
	static XMMATRIX sMatProjection;

	// 視点座標
	static XMFLOAT3 sEye;
	// 注視点座標
	static XMFLOAT3 sTarget;
	// 上方向ベクトル
	static XMFLOAT3 sUp;

};

