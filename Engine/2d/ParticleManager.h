#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>

class ParticleManager
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

	//頂点データ構造体
	struct VertexPos
	{
		XMFLOAT3 pos;	//xyz座標
		float scale;
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		//XMFLOAT4 color;	// 色 (RGBA)
		XMMATRIX mat;	// ３Ｄ変換行列
		XMMATRIX matBillboard;	//ビルボード行列
	};

private: // 定数
	static const int division = 50;					// 分割数
	static const float radius;				// 底面の半径
	static const float prizmHeight;			// 柱の高さ
	static const int planeCount = division * 2 + division * 2;		// 面の数
	static const int vertexCount = 1024;		// 頂点数

public: // 静的メンバ関数
	
	static void StaticInitialize(ID3D12Device* device, int window_width, int window_height);
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
	static void PostDraw();

	static const XMFLOAT3& GetEye() { return eye; }
	static const XMFLOAT3& GetTarget() { return target; }

	static void SetEye(XMFLOAT3 eye);
	static void SetTarget(XMFLOAT3 target);

	
	static void CameraMoveVector(XMFLOAT3 move);

	//ベクトルによる視点移動
	static void CameraMoveEyeVector(XMFLOAT3 move);

	static void SetCamera(XMFLOAT3& eye, XMFLOAT3& target, XMFLOAT3& up) {
		ParticleManager::eye = eye,
		ParticleManager::target = target,
		ParticleManager::up = up;
	}

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファ
	static ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
//	static ComPtr<ID3D12Resource> indexBuff;
	// テクスチャバッファ
	static ComPtr<ID3D12Resource> texbuff;
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// ビュー行列
	static XMMATRIX matView;
	// 射影行列
	static XMMATRIX matProjection;
	// 視点座標
	static XMFLOAT3 eye;
	// 注視点座標
	static XMFLOAT3 target;
	// 上方向ベクトル
	static XMFLOAT3 up;
	// 頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	// 頂点データ配列
	static VertexPos vertices[vertexCount];

	//ビルボード行列
	static XMMATRIX matBillborad;
	//Y軸ビルボード行列
	static XMMATRIX matBillboradY;

private:// 静的メンバ関数

	//デスクリプタヒープの初期化
	static void InitializeDescriptorHeap();

	//カメラ初期化
	static void InitializeCamera(int window_width, int window_height);

	//グラフィックパイプライン生成
	static void InitializeGraphicsPipeline();

	//テクスチャ読み込み
	static void LoadTexture();

	//モデル作成
	static void CreateModel();

	//ビュー行列を更新
	static void UpdateViewMatrix();

public: // メンバ関数

	bool Initialize();
	void Update();
	void Draw();

	void Add(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel,
		float start_scale, float end_scale);

private: // メンバ変数
	ComPtr<ID3D12Resource> constBuff; // 定数バッファ
	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };


	struct Particle
	{
		using XMFLOAT3 = DirectX::XMFLOAT3;

		XMFLOAT3 position = {};
		XMFLOAT3 velocity = {};
		XMFLOAT3 accel = {};
		int frame = 0;
		int num_frame = 0;
		float scale = 1.0f;
		float s_scale = 1.0f;
		float e_scale = 0.0f;
	};

	std::forward_list<Particle> particles;

};

const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs);

