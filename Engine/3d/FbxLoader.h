#pragma once

#include "fbxsdk.h"
#include "FbxModel.h"

#include <d3d12.h>
#include <d3dx12.h>
#include <string>

class FbxLoader
{
private:	//エイリアス
	//std::を省略
	using string = std::string;

public:	//定数

	//モデル格納ルートパス
	static const string sBaseDirectory;

public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();

	//初期化
	void Initialize(ID3D12Device* device);

	//後始末
	void Finalize();

	//ファイルからFBXモデル読み込み
	//void LoadModelFromFile(const string& modelName);

	void LoadModelFromFile(FbxModel* model, const string& modelName);

	//再帰的にノード構成を解析
	void ParseNodeRecursive(FbxModel* model, FbxNode* fbxNode, Node* parent = nullptr);

	//メッシュ読み取り
	void ParseMesh(FbxModel* model, FbxNode* fbxNode);

	//頂点座標読み取り
	void ParseMeshVertices(FbxModel* model, FbxMesh* fbxMesh);

	//面情報読み取り
	void ParseMeshFaces(FbxModel* model, FbxMesh* fbxMesh);

	//マテリアル読み取り
	void ParseMaterial(FbxModel* model, FbxNode* fbxNode);

	//テクスチャ読み込み
	void LoadTexture(FbxModel* model, const std::string& fullpath);

	//スキニング情報の読み取り
	void ParseSkin(FbxModel* model, FbxMesh* fbxMesh);


	//ディレクトリを含んだファイルパスからファイル名を抽出する
	std::string ExtractFileName(const std::string& path);

private:
	// privateなコンストラクタ
	FbxLoader() = default;
	// privateなデストラクタ
	~FbxLoader() = default;

	//D3D12デバイス
	ID3D12Device* device_ = nullptr;

	//FBXマネージャ
	FbxManager* fbxManager_ = nullptr;

	//FBXインポータ
	FbxImporter* fbxImporter_ = nullptr;

	//テクスチャがない場合の標準テクスチャファイル名
	static const string sDefaultTextureFileName;

public:

	static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src);

};