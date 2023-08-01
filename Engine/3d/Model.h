#pragma once

#include <DirectXMath.h>
#include <vector>
#include <string>
#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>
#include <unordered_map>

class Model
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // サブクラス
	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		DirectX::XMFLOAT3 pos; // xyz座標
		DirectX::XMFLOAT3 normal; // 法線ベクトル
		DirectX::XMFLOAT2 uv;  // uv座標
	};

	//定数バッファ用データ構造体B1
	struct ConstBufferDataB1
	{
		DirectX::XMFLOAT3 ambient;	//アンビエント係数
		float pad1;			//パディング
		DirectX::XMFLOAT3 diffuse;	//ディフューズ係数
		float pad2;			//パディング
		DirectX::XMFLOAT3 specular;	//スペキュラー係数
		float alpha;		//アルファ
	};

	//マテリアル
	struct Material
	{
		std::string name;
		DirectX::XMFLOAT3 ambient;
		DirectX::XMFLOAT3 diffuse;
		DirectX::XMFLOAT3 specular;
		float alpha;
		std::string textrueFilename;
		//コンストラクタ
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

public:



	//OBJファイルから3Dモデルを読み込む
	void LoadFromObj(const std::string& modelname, bool smoothing = false);

	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	void LoadTexture(const std::string& directoryPath, const std::string& filename);


	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial);

	static void SetDevice(ID3D12Device* device) { Model::sDevice = device; };

private:
	// デバイス(借りてくる)
	static ID3D12Device* sDevice;

	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize_ = 0;

	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff_;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap_;

	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV_;
	// シェーダリソースビューのハンドル(GPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV_;

	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff_;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff_;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView_;
	// マテリアル用定数バッファ
	ComPtr<ID3D12Resource> constBuffB1_; 

	// 頂点データ配列
	std::vector<VertexPosNormalUv> vertices_;

	// 頂点インデックス配列
	std::vector<unsigned short> indices_;

	//頂点法線スムージング用データ
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData_;

	
    // 頂点データの数を取得
	inline size_t GetVertexCount() { return vertices_.size(); }

    // エッジ平面化データの追加
	void AddSmoothData(unsigned short indexPosition, unsigned short indexVertex);

	//平面化された頂点法線の計算
	void CalculateSmoothedVertexNormals();

	//頂点法線スムージング用データ
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData;

	//マテリアル
	Material material;

	void LoadFromOBJInternal(const std::string& modelname,bool smoothing);

	//デスクリプタヒープの初期化
	void InitializeDescriptorHeap();

	//各種バッファの生成
	void CreateBuffers();
};