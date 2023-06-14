#pragma once

#include "Model.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#include "DirectionalLight.h"
#include "LightGroup.h"
#include <cstdint>

class Object3D
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス

	// 定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		//XMMATRIX mat;	// ３Ｄ変換行列

		XMMATRIX viewproj;	//ビュープロジェクション行列
		XMMATRIX world;		//ワールド行列
		XMFLOAT3 cameraPos;	//カメラ座標(ワールド座標)
	};

private: // 定数

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	static void StaticInitialize(ID3D12Device* device, int32_t window_width, int32_t window_height);

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	static Object3D* Create(float scale);

	/// <summary>
	/// 視点座標の取得
	/// </summary>
	/// <returns>座標</returns>
	static const XMFLOAT3& GetEye() { return sEye; }

	/// <summary>
	/// 視点座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	static void SetEye(XMFLOAT3& eye);

	/// <summary>
	/// 注視点座標の取得
	/// </summary>
	/// <returns>座標</returns>
	static const XMFLOAT3& GetTarget() { return sTarget; }

	static const XMFLOAT3& GetUp() { return sUp; }

	/// <summary>
	/// 注視点座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	static void SetTarget(XMFLOAT3& target);

	/// <summary>
	/// ベクトルによる移動
	/// </summary>
	/// <param name="move">移動量</param>
	static void CameraMoveVector(XMFLOAT3& move);

	/// <summary>
	/// ライトのセット
	/// </summary>
	/*static void SetLight(DirectionalLight* light) {
		Object3D::light = light;
	}*/

	static void SetLightGroup(LightGroup* lightGroup) {
		Object3D::sLightGroup = lightGroup;
	}

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* sDevice;
	// コマンドリスト
	static ID3D12GraphicsCommandList* sCmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> sRootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> sPipelinestate;
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

	//ライト
	//static DirectionalLight* light;
	static LightGroup* sLightGroup;

private:// 静的メンバ関数
	

	/// <summary>
	/// カメラ初期化
	/// </summary>
	/// <param name="window_width">画面横幅</param>
	/// <param name="window_height">画面縦幅</param>
	static void InitializeCamera(int32_t window_width, int32_t window_height);

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static void InitializeGraphicsPipeline();

	/// <summary>
	/// モデル作成
	/// </summary>
	static void CreateModel();

	/// <summary>
	/// ビュー行列を更新
	/// </summary>
	static void UpdateViewMatrix();

public: // メンバ関数
	bool Initialize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT3& GetPosition() const { return position_; }
	const XMFLOAT3& GetRotation() const { return rotation_; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const XMFLOAT3& position) { this->position_ = position; }
	void SetRotation(const XMFLOAT3& rotation) { this->rotation_ = rotation; }

	//setter
	void SetModel(Model* model) { model_ = model; };

private: // メンバ変数

	//3Dモデル(借りてくる)
	Model* model_ = nullptr;


	// 行列用定数バッファ
	ComPtr<ID3D12Resource> constBuffB0_; 
	// ローカルスケール
	XMFLOAT3 scale_ = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation_ = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position_ = { 0,0,0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld_ = {};
	// 親オブジェクト
	Object3D* parent_ = nullptr;

};

