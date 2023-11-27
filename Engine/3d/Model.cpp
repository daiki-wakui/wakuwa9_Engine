#include "Model.h"

#include <string>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <cassert>
#include <vector>
#include <DirectXMath.h>
#include <DirectXTex.h>


using namespace DirectX;
using namespace std;

ID3D12Device* Model::sDevice = nullptr;

void Model::LoadFromObj(const std::string& modelname, bool ddsTex, bool smoothing)
{
	//Model* model = new Model();

	//デスクリプタヒープ生成
	InitializeDescriptorHeap();

	//OBJファイルからデータを読み込む
	LoadFromOBJInternal(modelname, smoothing, ddsTex);

	//データを元にバッファ生成
	CreateBuffers();

	//return model;
}

void Model::AddSmoothData(unsigned short indexPosition, unsigned short indexVertex)
{
	smoothData_[indexPosition].emplace_back(indexVertex);
}

void Model::CalculateSmoothedVertexNormals()
{
	auto itr = smoothData_.begin();
	for (itr; itr != smoothData_.end(); ++itr) {
		//各面用の共通頂点コレクション
		std::vector<unsigned short>& v = itr->second;
		//全頂点の法線を平均する
		XMVECTOR normal = {};
		for (unsigned short index : v) {
			normal += XMVectorSet(vertices_[index].normal.x, vertices_[index].normal.y, vertices_[index].normal.z, 0);
		}
		normal = XMVector3Normalize(normal / (float)v.size());
		//共通法線を使用する全ての頂点データに書き込む
		for (unsigned short index : v) {
			vertices_[index].normal = { normal.m128_f32[0],normal.m128_f32[1],normal.m128_f32[2] };
		}
	}
}

void Model::LoadFromOBJInternal(const std::string& modelname, bool smoothing, bool ddsTex) {

	//ファイルストリーム
	std::ifstream file;
	//.objファイルを開く
	const string filename = modelname + ".obj";
	const string directoryPath = "Resources/" + modelname + "/";
	file.open(directoryPath + filename);

	//ファイルオープン失敗をチェック
	assert(!file.fail());

	vector<XMFLOAT3> positions;	//頂点座標
	vector<XMFLOAT3> normals;	//法線ベクトル
	vector<XMFLOAT2> texcoords;	//テクスチャUV

	//1行ずつ読み込む
	string line;
	while (getline(file, line)) {

		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		//半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		//先頭文字列がvなら頂点座標
		if (key == "v") {
			//X,Y,Z座標読み込み
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//座標データに追加
			positions.emplace_back(position);
		}

		//先頭文字列がvtならテクスチャ
		if (key == "vt") {
			//U,V成分読み込み
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V方向反転
			texcoord.y = 1.0f - texcoord.y;
			//テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}

		//先頭文字列がvnなら法線ベクトル
		if (key == "vn") {
			//X,Y,Z成分読み込み
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//法線ベクトルデータに追加
			normals.emplace_back(normal);
		}

		//先頭文字列がfならポリゴン(三角形)
		if (key == "f") {
			//半角スペース区切りで次の行の続きを読み込む
			string index_string;

			while (getline(line_stream, index_string, ' ')) {
				//頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);	//スラッシュを飛ばす
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);	//スラッシュを飛ばす
				index_stream >> indexNormal;

				//頂点データの追加
				VertexPosNormalUv vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertices_.emplace_back(vertex);

				//エッジ平面化用のデータを追加
				if (smoothing == true) {
					//vキー(座標データ)の番号と、全て合成したインデックスをセットで登録する
					AddSmoothData(indexPosition, (unsigned short)GetVertexCount() - 1);
				}

				//頂点インデックスに追加
				indices_.emplace_back((unsigned short)indices_.size());

			}
		}

		//先頭文字列がmtllibならマテリアル
		if (key == "mtllib") {
			//ファイル名の読み込み
			string rocalFilename;
			line_stream >> rocalFilename;

			//マテリアル読み込み
			LoadMaterial(directoryPath, rocalFilename, ddsTex);
		}
	}

	file.close();

	//頂点法線の平均によるヘッジの平面化
	if (smoothing == true) {
		CalculateSmoothedVertexNormals();
	}
}

void Model::LoadMaterial(const std::string& directoryPath, const std::string& filename, bool ddsTex)
{
	//ファイルストリーム
	std::ifstream file;
	//マテリアルファイルを開く
	file.open(directoryPath + filename);
	//ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	//1行ずつ読み込む
	string line;
	while (getline(file, line))
	{
		//1行分の文字列をストリームに変数
		std::istringstream line_stream(line);

		//半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		//先頭のタブ文字は無視する
		if (key[0] == '\t') {
			key.erase(key.begin());	//先頭の文字を削除
		}

		//先頭文字列がnewmtlならマテリアル名
		if (key == "newmtl") {
			//マテリアル名読み込み
			line_stream >> material.name;
		}

		//先頭文字列がKaならアンビエント色
		if (key == "Ka") {
			line_stream >> material.ambient.x;
			line_stream >> material.ambient.y;
			line_stream >> material.ambient.z;
		}

		//先頭文字列がKdならディフューズ色
		if (key == "Kd") {
			line_stream >> material.diffuse.x;
			line_stream >> material.diffuse.y;
			line_stream >> material.diffuse.z;
		}

		//先頭文字列がKsならスペキュラー色
		if (key == "Ks") {
			line_stream >> material.specular.x;
			line_stream >> material.specular.y;
			line_stream >> material.specular.z;
		}

		//先頭文字列がmap_Kdならテクスチャファイル名
		if (key == "map_Kd") {
			//テクスチャのファイル名読み込み
			line_stream >> material.textrueFilename;
			//テクスチャ読み込み
			LoadTexture(directoryPath, material.textrueFilename, ddsTex);
		}
	}

	//ファイルを閉じる
	file.close();
}

void Model::LoadTexture(const std::string& directoryPath, const std::string& filename, bool ddsTex)
{
	HRESULT result = S_FALSE;

	TexMetadata metadata{};
	ScratchImage scratchImg{};

	if (ddsTex) {
		int a = 0;
		a++;
	}

	//ファイルパスを統合
	string filepath = directoryPath + filename;
	//ユニコード文字列に変換する
	wchar_t wfilepath[128];
	int32_t iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));
	bufferSize_ = iBufferSize;

	// WICテクスチャのロード
	result = LoadFromWICFile(wfilepath, WIC_FLAGS_NONE, &metadata, scratchImg);
	assert(SUCCEEDED(result));

	ScratchImage mipChain{};
	// ミップマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	// 読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format, metadata.width, (UINT)metadata.height, (UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels);

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps =
		CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);

	// テクスチャ用バッファの生成
	result = sDevice->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr, IID_PPV_ARGS(&texbuff_));
	assert(SUCCEEDED(result));

	// テクスチャバッファにデータ転送
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		const Image* img = scratchImg.GetImage(i, 0, 0); // 生データ抽出
		result = texbuff_->WriteToSubresource(
			(UINT)i,
			nullptr,              // 全領域へコピー
			img->pixels,          // 元データアドレス
			(UINT)img->rowPitch,  // 1ラインサイズ
			(UINT)img->slicePitch // 1枚サイズ
		);
		assert(SUCCEEDED(result));
	}

	// シェーダリソースビュー作成
	cpuDescHandleSRV_ = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap_->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize_);
	gpuDescHandleSRV_ = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap_->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize_);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	D3D12_RESOURCE_DESC resDesc = texbuff_->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	sDevice->CreateShaderResourceView(texbuff_.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		cpuDescHandleSRV_
	);
}

void Model::InitializeDescriptorHeap()
{
	assert(sDevice);

	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 1; // シェーダーリソースビュー1つ
	result = sDevice->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap_));//生成
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// デスクリプタサイズを取得
	descriptorHandleIncrementSize_ = sDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

}

void Model::CreateBuffers() {
	HRESULT result = S_FALSE;

	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * vertices_.size());

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);

	// 頂点バッファ生成
	result = sDevice->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result));

	// 頂点バッファへのデータ転送
	VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		//memcpy(vertMap, vertices, sizeof(vertices));
		std::copy(vertices_.begin(), vertices_.end(), vertMap);
		vertBuff_->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeVB;
	vbView_.StrideInBytes = sizeof(vertices_[0]);

	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices_.size());

	// リソース設定
	resourceDesc.Width = sizeIB;

	// インデックスバッファ生成
	result = sDevice->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&indexBuff_));

	// インデックスバッファへのデータ転送
	unsigned short* indexMap = nullptr;
	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));
	if (SUCCEEDED(result)) {
		std::copy(indices_.begin(), indices_.end(), indexMap);
		indexBuff_->Unmap(0, nullptr);
	}

	// インデックスバッファビューの作成
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	ibView_.SizeInBytes = sizeIB;


	//定数バッファ用のリソース設定
	resourceDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff);

	// マテリアル用定数バッファの生成
	result = sDevice->CreateCommittedResource(
		&heapProps, // アップロード可能
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffB1_));
	assert(SUCCEEDED(result));

	//マテリアル用定数バッファへデータ転送
	ConstBufferDataB1* constMap1 = nullptr;
	result = constBuffB1_->Map(0, nullptr, (void**)&constMap1);
	if (SUCCEEDED(result)) {
		constMap1->ambient = material.ambient;
		constMap1->diffuse = material.diffuse;
		constMap1->specular = material.specular;
		constMap1->alpha = material.alpha;
		constBuffB1_->Unmap(0, nullptr);
	}

}

void Model::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial) {
	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView_);
	// インデックスバッファの設定
	cmdList->IASetIndexBuffer(&ibView_);
	// マテリアル用定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(rootParamIndexMaterial, constBuffB1_->GetGPUVirtualAddress());

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap_.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	if (material.textrueFilename.size() > 0) {
		// シェーダリソースビューをセット
		cmdList->SetGraphicsRootDescriptorTable(2, gpuDescHandleSRV_);
	}

	// 描画コマンド
	cmdList->DrawIndexedInstanced((UINT)indices_.size(), 1, 0, 0, 0);
}
