﻿#pragma once

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
	static const string baseDirectory;

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

	FbxModel* LoadModelFromFile(const string& modelName);

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

	//ディレクトリを含んだファイルパスからファイル名を抽出する
	std::string ExtractFileName(const std::string& path);

private:
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;


	//D3D12デバイス
	ID3D12Device* device = nullptr;

	//FBXマネージャ
	FbxManager* fbxManager = nullptr;

	//FBXインポータ
	FbxImporter* fbxImporter = nullptr;

	//テクスチャがない場合の標準テクスチャファイル名
	static const string defaultTextureFileName;
};