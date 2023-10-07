#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include <DirectXTex.h>

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <fbxsdk.h>
#include <cstdint>

struct Node
{
	//名前
	std::string name;
	float padding[2];

	//ローカルスケール
	DirectX::XMVECTOR scaling = { 1,1,1,0 };

	//ローカル回転角
	DirectX::XMVECTOR rotation = { 0,0,0,0 };

	//ローカル移動
	DirectX::XMVECTOR translation = { 0,0,0,1 };

	//ローカル変形行列
	DirectX::XMMATRIX transform = {};

	//グローバル変形行列
	DirectX::XMMATRIX globalTransform = {};

	//親ノード
	Node* parent = nullptr;
};

class FbxModel
{
	

private: //エイリアス

	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	//std::を省略
	using string = std::string;
	template <class T> using vector = std::vector<T>;


public: //サブクラス

	//ボーンインデックスの最大数
	static const int32_t sMAX_BONE_INDICES = 4;


	//頂点データ構造体
	struct VertexPosNormalUvSkin
	{
		DirectX::XMFLOAT3 pos;	//xyz座標
		DirectX::XMFLOAT3 normal;	//法線ベクトル
		DirectX::XMFLOAT2 uv;	//uv座標
		UINT boneIndex[sMAX_BONE_INDICES];	//ボーン 番号
		float boneWeight[sMAX_BONE_INDICES];	//ボーン　重み
	};

	//ボーン構造体
	struct Bone
	{
		//名前
		std::string name;

		//初期姿勢の逆行列
		DirectX::XMMATRIX invInitialPose;

		//クラスター
		FbxCluster* fbxCluster;

		//コンストラクタ
		Bone(const std::string& name) {
			this->name = name;
		}
	};

public:

	//フレンドクラス
	friend class FbxLoder;

	//モデル名
	std::string name_;

	//ノード配列
	std::vector<Node> nodes_;

	//メッシュを持つノード
	Node* meshNode = nullptr;

	//頂点データ配列
	std::vector<VertexPosNormalUvSkin> vertices_;

	//頂点インデックス配列
	std::vector<unsigned short> indices_;

	//アンビエント係数
	DirectX::XMFLOAT3 ambient_ = { 1,1,1 };

	//ディディフューズ係数
	DirectX::XMFLOAT3 diffuse_ = { 1,1,1 };

	//テクスチャメタデータ
	DirectX::TexMetadata metadata_ = {};

	//スクラッチイメージ
	DirectX::ScratchImage scratchImg_ = {};

	//ボーン配列
	std::vector<Bone> bones_;

	//FBXシーン
	FbxScene* fbxScene_ = nullptr;

	
private: //メンバ変数

	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff_;
	//インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff_;
	//テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff_;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_ = {};
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView_ = {};
	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV_;


public: //メンバ関数

	//getter
	std::vector<Bone>& GetBones() { return bones_; }

	//getter
	FbxScene* GetFbxScene() { return fbxScene_; }

	//デストラクタ
	~FbxModel();

	void CreateBuffers(ID3D12Device* device);

	void Draw(ID3D12GraphicsCommandList* cmdList);

	//モデルの変形行列情報
	const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }

	
};

