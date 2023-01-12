#include "WindowsApp.h"
#include "KeyBoard.h"
#include "DirectXBasis.h"
#include "Object3D.h"
#include "Model.h"
#include "Player.h"
#include "Enemy.h"

#include <memory>
#include <string>
#include <DirectXTex.h>

#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include <xaudio2.h>
#pragma comment (lib,"xaudio2.lib")
#include <fstream>
#include <wrl.h>

#include <memory>
#include <list>

//チャンクヘッダ
struct ChunkHeader
{
	char id[4];	//チャンクごとのid
	int32_t size;	//チャンクサイズ
};

//RIFFヘッダチャンク
struct RiffHeader
{
	ChunkHeader chunk;	//RIFF
	char type[4];	//WAVE
};

//FMTチャンク
struct FormatChunk
{
	ChunkHeader chunk;	//fmt
	WAVEFORMATEX fmt;	//波形フォーマット
};

//音声データ
struct SoundData
{
	//波形フォーマット
	WAVEFORMATEX wfex;
	//バッファの先頭アドレス
	BYTE* pBuffer;
	//バッファのサイズ
	unsigned int bufferSize;

	IXAudio2SourceVoice* pSoundVoice_;
};

SoundData SoundLoadWave(const char* filename) {
	HRESULT result;

	//ファイルオープン
	//ファイル入力ストリームのインスタンス
	std::ifstream file;

	//.wavファイルをバイナリモードで開く
	file.open(filename, std::ios_base::binary);
	//ファイルオープン失敗をチェック
	assert(file.is_open());

	//wav読み込み
	//RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	//ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	//タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	//Formatチャンクの読み込み
	FormatChunk format = {};
	//チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	/*if (strncmp(format.chunk.id, "fmt", 4) != 0) {
		assert(0);
	}*/
	//チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	//Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));

	//JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK", 4) == 0) {
		//読み込み位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		//再読み込み
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	//Dataチャンクのデータ部(波形データ)の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	//ファイルクローズ
	file.close();

	//読み込んだ音声データをretrun
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	return soundData;
}

//音声データ解放
void SoundUnload(SoundData* soundData) {
	//バッファのメモリを解放
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

//音声再生
void SoundPlayWave(IXAudio2* xAudio2, SoundData& soundData) {
	HRESULT result;

	//波形フォーマットを元にSourceVoiceの生成
	IXAudio2SourceVoice* pSoundVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSoundVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	//再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	soundData.pSoundVoice_ = pSoundVoice;

	//波形データの再生
	result = pSoundVoice->SubmitSourceBuffer(&buf);
	result = pSoundVoice->Start();

	//
}

void SoundStopWAVE(IXAudio2* xAudio2, const SoundData& soundData) {
	soundData.pSoundVoice_->Stop();
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//windowsAPIの生成クラス
	std::unique_ptr<WindowsApp> windows;
	WindowsApp* winApp = new WindowsApp();

	//keyboradクラスの生成
	std::unique_ptr<KeyBoard> keyboard;
	KeyBoard* input_ = new KeyBoard();

	//DirectXの基盤生成クラス
	std::unique_ptr<DirectXBasis> DirectX;
	DirectXBasis* dxBasis = new DirectXBasis();

	//windowsAPI初期化
	winApp->Initalize();
	windows.reset(winApp);
	MSG msg{};

	Microsoft::WRL::ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;

	//DirectX初期化
	dxBasis->Initialize(winApp);
	DirectX.reset(dxBasis);

	HRESULT result = S_FALSE;
	//オーディオ初期化
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	result = xAudio2->CreateMasteringVoice(&masterVoice);

	Object3D::StaticInitialize(dxBasis->GetDevice(), winApp->GetWindowWidth(), winApp->GetWindowHeight());

	//keyborad初期化
	input_->Initialize(winApp->GetHInstancee(), winApp->GetHwnd());
	keyboard.reset(input_);

	

#pragma region  描画初期化処理
	//HRESULT result;

#pragma region  画像イメージデータ

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	//WICテクスチャのロード
	result = LoadFromWICFile(
		L"Resources/test.png",
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

#pragma endregion

#pragma region  ミニマップの生成

	ScratchImage mipChain{};
	//ミニマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages(),
		scratchImg.GetImageCount(),
		scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT,0,mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

#pragma endregion

#pragma region  フォーマットを書き換える
	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

#pragma endregion

#pragma region  テクスチャバッファ設定

	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (UINT)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;


#pragma endregion

#pragma region  テクスチャバッファの生成

	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff = nullptr;
	result = dxBasis->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));

#pragma endregion

#pragma region  テクスチャバッファへのデータ転送
	//データ転送
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		const Image* img = scratchImg.GetImage(i, 0, 0);
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);
		assert(SUCCEEDED(result));
	}

#pragma endregion

#pragma region  テクスチャサンプラーの設定
	//テクスチャからどのように色を取り出すかの設定
	//テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	//横繰り返し
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	//縦繰り返し
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	//奥行繰り返し
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;	//ミニマップ最大値
	samplerDesc.MinLOD = 0.0f;	//ミニマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

#pragma endregion

#pragma region  デスクリプタヒープ生成

	//SRVの最大個数
	const size_t kMaxSRVCount = 2056;

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;
	result = dxBasis->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));

	//SRVヒープの先頭アドレスを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

#pragma endregion

#pragma region  シェーダリソースビューの生成

	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	//ハンドルの指す位置にシェーダリソースビュー作成
	dxBasis->GetDevice()->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvHandle);

#pragma endregion

#pragma region  デスクリプタレンジの設定
	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;	//テクスチャレジスタ0番
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
#pragma endregion


#pragma region  定数バッファ

	//定数バッファ用データ構造体(マテリアル)
	struct ConstBufferDataMaterial{
		XMFLOAT4 color;	//色(RGBA)
	};

	//定数バッファ用データ構造体(3D変換行列)
	struct CounstBufferDataTransform{
		XMMATRIX mat;	//3D変換行列
	};

	CounstBufferDataTransform* constMapTransform = nullptr;

#pragma region  定数バッファの設定

	//ヒープ設定(color)
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	
	//ヒープ設定(mat)
	D3D12_HEAP_PROPERTIES cbMatHeapProp{};
	cbMatHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC cbMatResourceDesc{};
	cbMatResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbMatResourceDesc.Width = (sizeof(CounstBufferDataTransform) + 0xff) & ~0xff;
	cbMatResourceDesc.Height = 1;
	cbMatResourceDesc.DepthOrArraySize = 1;
	cbMatResourceDesc.MipLevels = 1;
	cbMatResourceDesc.SampleDesc.Count = 1;
	cbMatResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	

#pragma endregion

#pragma region  定数バッファの生成

	//color
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
	//定数バッファの生成
	result = dxBasis->GetDevice()->CreateCommittedResource(
		&cbHeapProp,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result));

	//mat
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	//定数バッファの生成
	result = dxBasis->GetDevice()->CreateCommittedResource(
		&cbMatHeapProp,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbMatResourceDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform));
	assert(SUCCEEDED(result));


#pragma endregion

#pragma region	定数バッファのマッピング

	//定数バッファのマッピング(color)
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);	//マッピング
	assert(SUCCEEDED(result));

	//定数バッファのマッピング(mat)
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));

#pragma endregion

#pragma region	定数バッファへのデータ転送

	//値を書きこんで自動転送
	constMapMaterial->color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f);	//色変更


#pragma region	平行投影変換
	//平行投影変換
	constMapTransform->mat = XMMatrixIdentity();

	XMMATRIX matProjection=
		XMMatrixOrthographicOffCenterLH(
			0.0f, winApp->GetWindowWidth(),
			winApp->GetWindowHeight(), 0.0f,
			0.0f, 1.0f);

	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();

	XMMATRIX matScale;
	matScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	matWorld *= matScale;

	XMMATRIX matRot;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(0.0f));	//Z 0度回転
	matRot *= XMMatrixRotationX(XMConvertToRadians(0.0f));	//X 0度回転
	matRot *= XMMatrixRotationY(XMConvertToRadians(0.0f));	//Y 0度回転
	matWorld *= matRot;

	XMMATRIX matTrans;
	matTrans = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	matWorld *= matTrans;

	constMapTransform->mat = matWorld * matProjection;

	XMFLOAT2 pos = { 0,0 };

#pragma endregion

#pragma endregion

#pragma endregion

#pragma region  頂点データ

	//頂点データ構造体
	struct Vertex{
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

	// 頂点データ
	Vertex vertices[] = {
		{{   0.0f, 100.0f, 0.0f },{ 0.0f , 1.0f }}, // 左下
		{{   0.0f,   0.0f, 0.0f },{ 0.0f , 0.0f }}, // 左下
		{{ 100.0f, 100.0f, 0.0f },{ 1.0f , 1.0f }}, // 左下
		{{ 100.0f,   0.0f, 0.0f },{ 1.0f , 0.0f }}, // 左下
	};

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

#pragma endregion

#pragma region  インデックスデータ

	//インデックスデータ
	uint16_t indices[] = {
		0 , 1 , 2,	//三角形1つ目
		1 , 2 , 3,	//三角形2つ目
	};

#pragma endregion

#pragma region  頂点バッファの設定
	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

#pragma endregion

#pragma region  頂点バッファの生成

	// 頂点バッファの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;
	result = dxBasis->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

#pragma endregion

#pragma region  頂点バッファへのデータ転送

	//GPU上のバッファに対応した仮想メモリ
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//全頂点に対して
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i];
	}
	//繋がりを解除
	vertBuff->Unmap(0, nullptr);

#pragma endregion

#pragma region  頂点バッファビューの生成

	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);

#pragma endregion

#pragma region  インデックスバッファの設定

	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	//リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;	//インデックス情報が入る分のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

#pragma endregion

#pragma region  インデックスバッファの生成

	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff = nullptr;
	result = dxBasis->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

#pragma endregion

#pragma region  インデックスバッファへのデータ転送

	//インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//全インデックスに対して
	for (int i = 0; i < _countof(indices); i++) {
		indexMap[i] = indices[i];
	}
	//マッピング解除
	indexBuff->Unmap(0, nullptr);

#pragma endregion

#pragma region  インデックスバッファビューの生成

	//インデックスバッファビューの生成
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

#pragma endregion

#pragma region  頂点シェーダーファイルの読み込み

	ID3DBlob* vsBlob = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob = nullptr; // エラーオブジェクト
	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/BasicVShader.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);

	// エラーなら
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

#pragma endregion

#pragma region  ピクセルシェーダファイルの読み込み

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/BasicPShader.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);

	// エラーなら
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

#pragma endregion

#pragma region  頂点レイアウト

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{	//xyz座標
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{	//uv座標
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

#pragma endregion

#pragma region  グラフィックスパイプライン設定

#pragma region  グラフィックスパイプラインに必要な設定

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// シェーダーの設定
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// ラスタライザの設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // カリングしない
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

#pragma endregion

#pragma region  ブレンド設定

	// ブレンドステート
	//レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	//RGBA全てのチャンネルを描画

	//共通設定(アルファ値)
	blenddesc.BlendEnable = true;	//ブレンドを有効
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;	//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を0%使う

	//加算合成
	/*blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_ONE;*/

	//減算合成
	/*blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_ONE;*/

	//色反転
	/*blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
	blenddesc.DestBlend = D3D12_BLEND_ZERO;*/

	//半透明合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

#pragma endregion

#pragma region  頂点レイアウト・その他形状設定

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// その他の設定
	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

#pragma endregion

#pragma region  ルートパラメータ

	//ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams[3];
	//定数バッファ0番
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootParams[0].Descriptor.ShaderRegister = 0;	//定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;		//デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える
	//テクスチャレジスタ0番
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//定数バッファ1番
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootParams[2].Descriptor.ShaderRegister = 1;	//定数バッファ番号
	rootParams[2].Descriptor.RegisterSpace = 0;		//デフォルト値
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える
#pragma endregion

#pragma region  ルートシグネチャ

	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;
	rootSignatureDesc.NumParameters = _countof(rootParams);
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ルートシグネチャのシリアライズ
	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = dxBasis->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();
	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature.Get();

#pragma endregion
	
#pragma region  パイプラインステート生成

	//パイプラインステートの生成
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState = nullptr;
	result = dxBasis->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

#pragma endregion

#pragma endregion

#pragma endregion

	//音
	SoundData soundData1 = SoundLoadWave("Resources/Sound/PerituneMaterial.wav");
	SoundData soundData2 = SoundLoadWave("Resources/Sound/Hit.wav");
	SoundData soundData3 = SoundLoadWave("Resources/Sound/Electric Wild.wav");
	SoundData soundData4 = SoundLoadWave("Resources/Sound/Shot.wav");
	SoundData soundData5 = SoundLoadWave("Resources/Sound/damage.wav");
	SoundData soundData6 = SoundLoadWave("Resources/Sound/dash.wav");

	int PlayBGM = 0;
	int ChangeBGM = 0;

	//OBJからモデルを読み込む
	Model* model = Model::LoadFromObj("boss");
	Model* model2 = Model::LoadFromObj("world");

	Model* playerModel = Model::LoadFromObj("Cube");
	//Model* playerBulletModel = Model::LoadFromObj("playerBullet");
	Model* floorModel = Model::LoadFromObj("Floor");

	Model* fieldBlock = Model::LoadFromObj("bobj");
	Model* fieldBlock2 = Model::LoadFromObj("aobj");

	Model* playerModel_ = Model::LoadFromObj("player");
	Model* podModel_ = Model::LoadFromObj("pad");
	Model* enemyModel_ = Model::LoadFromObj("enemy");

	Model* UIModel[13];
	UIModel[0] = Model::LoadFromObj("gameover");
	UIModel[1] = Model::LoadFromObj("kaihi");
	UIModel[2] = Model::LoadFromObj("move");

	UIModel[3] = Model::LoadFromObj("rot");
	UIModel[4] = Model::LoadFromObj("spaceStart");
	UIModel[5] = Model::LoadFromObj("spaceTitle");
	UIModel[6] = Model::LoadFromObj("wave13");
	UIModel[7] = Model::LoadFromObj("wave23");
	UIModel[8] = Model::LoadFromObj("wave33");
	UIModel[9] = Model::LoadFromObj("retry");
	UIModel[10] = Model::LoadFromObj("auto");
	UIModel[11] = Model::LoadFromObj("buster");
	UIModel[12] = Model::LoadFromObj("gameclear");

#pragma region  オブジェクト生成

	//UI
	Object3D* UIobj[13];

	for (int i = 0; i < 13; i++) {
		UIobj[i] = Object3D::Create(UIModel[i], { 3,3,3 });
	}

	//タイトルのUI
	UIobj[10]->SetPosition({ -40,20,50 });
	UIobj[11]->SetPosition({ 30,10,25 });

	//3Dオブジェクト生成
	Object3D* object3d3 = Object3D::Create(model2, { (500.0f),(500.0f),(500.0f) });

	Object3D* enemyObject = Object3D::Create(enemyModel_, { (7.0f),(7.0f),(7.0f) });
	Object3D* enemyObject2 = Object3D::Create(enemyModel_, { (7.0f),(7.0f),(7.0f) });
	Object3D* enemyObject3 = Object3D::Create(enemyModel_, { (7.0f),(7.0f),(7.0f) });

	Object3D* enemyObject4 = Object3D::Create(enemyModel_, { (7.0f),(7.0f),(7.0f) });
	Object3D* enemyObject5 = Object3D::Create(enemyModel_, { (7.0f),(7.0f),(7.0f) });
	Object3D* enemyObject6 = Object3D::Create(enemyModel_, { (7.0f),(7.0f),(7.0f) });
	Object3D* enemyObject7 = Object3D::Create(enemyModel_, { (7.0f),(7.0f),(7.0f) });


	Object3D* playerObject = Object3D::Create(playerModel_, { (2.0f),(2.0f),(2.0f) });
	Object3D* podObject = Object3D::Create(podModel_, { (2),(2),(2) });

	Object3D* floorObject = Object3D::Create(floorModel, { (25.0f),(25.0f),(25.0f) });
	Object3D* fieldblock = Object3D::Create(fieldBlock, { (10.0f),(10.0f),(10.0f) });
	Object3D* fieldblock2 = Object3D::Create(fieldBlock, { (15.0f),(10.0f),(10.0f) });
	Object3D* fieldblock3 = Object3D::Create(fieldBlock, { (10.0f),(20.0f),(10.0f) });
	Object3D* fieldblock4 = Object3D::Create(fieldBlock2, { (10.0f),(10.0f),(10.0f) });
	Object3D* fieldblock5 = Object3D::Create(fieldBlock2, { (15.0f),(15.0f),(15.0f) });

	Object3D* fieldblock6 = Object3D::Create(fieldBlock, { (10.0f),(10.0f),(10.0f) });
	Object3D* fieldblock7 = Object3D::Create(fieldBlock, { (15.0f),(10.0f),(10.0f) });
	Object3D* fieldblock8 = Object3D::Create(fieldBlock, { (10.0f),(20.0f),(10.0f) });
	Object3D* fieldblock9 = Object3D::Create(fieldBlock2, { (10.0f),(10.0f),(10.0f) });
	Object3D* fieldblock10 = Object3D::Create(fieldBlock2, { (15.0f),(15.0f),(15.0f) });

	Object3D* fieldblock11 = Object3D::Create(fieldBlock, { (10.0f),(10.0f),(10.0f) });
	Object3D* fieldblock12 = Object3D::Create(fieldBlock, { (15.0f),(10.0f),(10.0f) });
	Object3D* fieldblock13 = Object3D::Create(fieldBlock, { (10.0f),(20.0f),(10.0f) });
	Object3D* fieldblock14 = Object3D::Create(fieldBlock2, { (10.0f),(10.0f),(10.0f) });
	Object3D* fieldblock15 = Object3D::Create(fieldBlock2, { (15.0f),(15.0f),(15.0f) });

	playerObject->SetPosition({ 0,0,-20 });
	floorObject->SetPosition({ 0,-10,0 });

	fieldblock->SetPosition({ -40,40,300 });
	fieldblock2->SetPosition({ 150,20,300 });
	fieldblock3->SetPosition({ -150,80,300 });
	fieldblock4->SetPosition({ 30,100,300 });
	fieldblock5->SetPosition({ -100,200,300 });

	fieldblock6->SetPosition({ 300,40,120 });
	fieldblock7->SetPosition({ 300,20,0 });
	fieldblock8->SetPosition({ 300,80,-50 });
	fieldblock9->SetPosition({ 270,100,280 });
	fieldblock10->SetPosition({ 300,150,200 });

	fieldblock11->SetPosition({ -300,40,200 });
	fieldblock12->SetPosition({ -300,120,280 });
	fieldblock13->SetPosition({ -300,80,-20 });
	fieldblock14->SetPosition({ -250,20,-100 });
	fieldblock15->SetPosition({ -350,150,150 });


	fieldblock4->SetRotation({ 0,90,0 });
	fieldblock5->SetRotation({ 0,-90,0 });
	fieldblock9->SetRotation({ 0,90,0 });
	fieldblock14->SetRotation({ 0,-90,0 });
#pragma endregion

	Object3D::CameraMoveVector({ 0.0f,20.0f,-30.0f });

	float angle = 0.0f;

	Player* player = new Player;
	player->Initialize(playerModel_, playerObject, input_, podObject);

	int isPlayerDamege = 0;
	int invincibleTime = 0;
	int invincible = 0;

	int wave = 0;
	std::list<std::unique_ptr<Enemy>> enemys_;

	int isPop = 0;
	int scene = 0;
	int knockDownNum = 0;
	int popCoolTime = 0;
	XMFLOAT3 domeRot={};

	//ゲームループ
	while (true) {
		//×ボタンで終了メッセージがきたら
		if (winApp->gameloopExit(msg) == true) {
			break;	//ゲームループ終了
		}

		//-----ここから更新処理-----//
		//シーン切り替え
		//タイトル
		if (scene == 0) {
			if (input_->keyInstantPush(DIK_SPACE)) {
				scene = 1;
				player->HP = 5;
				player->isDead = false;
				playerObject->SetPosition({ 0,0,-20 });
				playerObject->SetRotation({ 0,0,0 });
				Object3D::SetCamera();
				enemys_.clear();
				player->clear();
				wave = 0;
				isPop = 0;
				knockDownNum = 0;
				popCoolTime = 0;
				isPlayerDamege = 0;
				invincibleTime = 0;
				invincible = 0;
			}
		}
		//ゲームシーン
		else if (scene == 1) {
			if (input_->keyInstantPush(DIK_SPACE)) {
				scene = 2;
			}
		}
		//ゲームオーバー
		else if (scene == 2) {
			if (input_->keyInstantPush(DIK_SPACE)) {
				scene = 1;
				player->HP = 5;
				player->isDead = false;
				playerObject->SetPosition({ 0,0,-20 });
				playerObject->SetRotation({ 0,0,0 });
				Object3D::SetCamera();
				enemys_.clear();
				player->clear();
				wave = 0;
				isPop = 0;
				knockDownNum = 0;
				popCoolTime = 0;
				isPlayerDamege = 0;
				invincibleTime = 0;
				invincible = 0;
			}
		}
		//ゲームクリア
		else if (scene == 3) {
			if (input_->keyInstantPush(DIK_SPACE)) {
				scene = 0;
			}
		}

		/*if (PlayBGM == 0 && ChangeBGM == 0) {
			SoundPlayWave(xAudio2.Get(), soundData1);
			PlayBGM = 1;
		}
		if (PlayBGM == 0 && ChangeBGM == 1) {
			SoundPlayWave(xAudio2.Get(), soundData3);
			PlayBGM = 1;
		}

		if (input_->keyInstantPush(DIK_G)) {
			if (ChangeBGM == 0) {
				SoundStopWAVE(xAudio2.Get(), soundData1);
			}
			PlayBGM = 0;
			ChangeBGM = 1;
		}*/

		//keyborad更新処理
		input_->Update();

#pragma region  オブジェクト更新処理

		/*if (isPlayerDamege == 1) {
			domeRot = object3d3->GetRotation();

			domeRot.y += 50;

			object3d3->SetRotation(domeRot);
		}*/

		if (scene == 0) {
			for (int i = 0; i < 13; i++) {
				UIobj[i]->Update();
			}
		}

		object3d3->Update();

		if (scene == 1) {

			if (input_->keyInstantPush(DIK_K)) {
				player->OnCollision();
			}

			if (player->IsDead() == true) {
				scene = 2;
			}

#pragma region  ウェーブ管理

			//チュートリアル
			if (wave == 0) {
				if (isPop == 0) {
					std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
					std::unique_ptr<Enemy> newEnemy2 = std::make_unique<Enemy>();
					std::unique_ptr<Enemy> newEnemy3 = std::make_unique<Enemy>();

					newEnemy->Initialize(enemyObject, { -70,0,100 }, player);
					newEnemy2->Initialize(enemyObject2, { 0,0,100 }, player);
					newEnemy3->Initialize(enemyObject3, { 70,0,100 }, player);

					//敵を登録する
					enemys_.push_back(std::move(newEnemy));
					enemys_.push_back(std::move(newEnemy2));
					enemys_.push_back(std::move(newEnemy3));
					isPop++;
				}

				if (knockDownNum == 3) {
					wave++;
					knockDownNum = 0;
					isPop = 0;
				}
			}

			//ウェーブ1
			if (wave == 1) {
				popCoolTime++;

				if (popCoolTime >= 300) {
					if (isPop == 0) {
						std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
						std::unique_ptr<Enemy> newEnemy2 = std::make_unique<Enemy>();
						std::unique_ptr<Enemy> newEnemy3 = std::make_unique<Enemy>();

						newEnemy->Initialize(enemyObject, { -70,0,50 }, player);
						newEnemy2->Initialize(enemyObject2, { 0,0,100 }, player);
						newEnemy3->Initialize(enemyObject3, { 70,0,50 }, player);

						//敵を登録する
						enemys_.push_back(std::move(newEnemy));
						enemys_.push_back(std::move(newEnemy2));
						enemys_.push_back(std::move(newEnemy3));
						popCoolTime = 0;
						isPop = 1;
					}
				}

				if (popCoolTime >= 100) {
					if (isPop == 1) {
						std::unique_ptr<Enemy> newEnemy4 = std::make_unique<Enemy>();
						std::unique_ptr<Enemy> newEnemy5 = std::make_unique<Enemy>();
						std::unique_ptr<Enemy> newEnemy6 = std::make_unique<Enemy>();

						newEnemy4->Initialize(enemyObject4, { -90,0,100 }, player);
						newEnemy5->Initialize(enemyObject5, { 0,0,50 }, player);
						newEnemy6->Initialize(enemyObject6, { 90,0,100 }, player);

						//敵を登録する
						enemys_.push_back(std::move(newEnemy4));
						enemys_.push_back(std::move(newEnemy5));
						enemys_.push_back(std::move(newEnemy6));
						isPop++;
					}
				}

				//全部倒したら
				if (knockDownNum == 6) {
					wave++;
					knockDownNum = 0;
					popCoolTime = 0;
					isPop = 0;
				}
			}

			//ウェーブ2
			if (wave == 2) {
				popCoolTime++;

				if (popCoolTime >= 300 && isPop == 0) {
					std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
					std::unique_ptr<Enemy> newEnemy2 = std::make_unique<Enemy>();
					std::unique_ptr<Enemy> newEnemy3 = std::make_unique<Enemy>();
					std::unique_ptr<Enemy> newEnemy4 = std::make_unique<Enemy>();
					std::unique_ptr<Enemy> newEnemy5 = std::make_unique<Enemy>();
					std::unique_ptr<Enemy> newEnemy6 = std::make_unique<Enemy>();

					newEnemy->Initialize(enemyObject, { -70,0,20 }, player);
					newEnemy2->Initialize(enemyObject2, { -50,0,60 }, player);
					newEnemy3->Initialize(enemyObject3, { -20,0,100 }, player);
					newEnemy4->Initialize(enemyObject4, { 20,0,100 }, player);
					newEnemy5->Initialize(enemyObject5, { 50,0,60 }, player);
					newEnemy6->Initialize(enemyObject6, { 70,0,20 }, player);

					//敵を登録する
					enemys_.push_back(std::move(newEnemy));
					enemys_.push_back(std::move(newEnemy2));
					enemys_.push_back(std::move(newEnemy3));
					enemys_.push_back(std::move(newEnemy4));
					enemys_.push_back(std::move(newEnemy5));
					enemys_.push_back(std::move(newEnemy6));

					isPop++;
				}

				//全部倒したら
				if (knockDownNum == 6) {
					wave++;
					knockDownNum = 0;
					popCoolTime = 0;
					isPop = 0;
				}
			}

			//ウェーブ3
			if (wave == 3) {
				popCoolTime++;

				if (popCoolTime >= 300 && isPop == 0) {
					std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();

					newEnemy->Initialize(enemyObject, { -70,0,0 }, player);
					//敵を登録する
					enemys_.push_back(std::move(newEnemy));
					popCoolTime = 0;
					isPop++;
				}

				if (popCoolTime >= 50 && isPop == 1) {
					std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();

					newEnemy->Initialize(enemyObject2, { 70,0,30 }, player);
					//敵を登録する
					enemys_.push_back(std::move(newEnemy));
					popCoolTime = 0;
					isPop++;
				}

				if (popCoolTime >= 50 && isPop == 2) {
					std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();

					newEnemy->Initialize(enemyObject3, { -50,0,60 }, player);
					//敵を登録する
					enemys_.push_back(std::move(newEnemy));
					popCoolTime = 0;
					isPop++;
				}

				if (popCoolTime >= 50 && isPop == 3) {
					std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();

					newEnemy->Initialize(enemyObject4, { 50,0,90 }, player);
					//敵を登録する
					enemys_.push_back(std::move(newEnemy));
					popCoolTime = 0;
					isPop++;
				}

				if (popCoolTime >= 50 && isPop == 4) {
					std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();

					newEnemy->Initialize(enemyObject5, { -20,0,120 }, player);
					//敵を登録する
					enemys_.push_back(std::move(newEnemy));
					popCoolTime = 0;
					isPop++;
				}

				if (popCoolTime >= 50 && isPop == 5) {
					std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();

					newEnemy->Initialize(enemyObject6, { 20,0,120 }, player);
					//敵を登録する
					enemys_.push_back(std::move(newEnemy));
					popCoolTime = 0;
					isPop++;
				}

				if (popCoolTime >= 300 && isPop == 6 && knockDownNum == 6) {
					std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();

					newEnemy->Initialize(enemyObject7, { 0,0,100 }, player,15);
					//敵を登録する
					enemys_.push_back(std::move(newEnemy));
					popCoolTime = 0;
					isPop++;
				}

				if (knockDownNum == 7) {
					scene = 3;
				}
			}

#pragma endregion

			floorObject->Update();

			fieldblock->Update();
			fieldblock2->Update();
			fieldblock3->Update();
			fieldblock4->Update();
			fieldblock5->Update();
			fieldblock6->Update();
			fieldblock7->Update();
			fieldblock8->Update();
			fieldblock9->Update();
			fieldblock10->Update();
			fieldblock11->Update();
			fieldblock12->Update();
			fieldblock13->Update();
			fieldblock14->Update();
			fieldblock15->Update();

			player->Update();

			//発射SE
			if (player->GetCoolTime() == 0) {
				SoundPlayWave(xAudio2.Get(), soundData4);
			}

			if (player->GetIsStep() == true && input_->keyInstantPush(DIK_W)) {
				SoundPlayWave(xAudio2.Get(), soundData6);
			}

			//
			//無敵時間
			if (player->GetIsStep() == true) {
				invincible = true;
			}
			else {
				invincible = false;
			}

			if (isPlayerDamege == 1) {
				invincibleTime++;

				if (invincibleTime >= 100) {
					isPlayerDamege = 0;
					invincibleTime = 0;
				}
			}

			//enemyの死亡フラグ
			enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) {
				return enemy->IsDead();
			});

			//敵の動き
			for (std::unique_ptr<Enemy>& enemy : enemys_) {
				enemy->Update();
			}
		}
		
		if (scene == 2) {
			floorObject->Update();
			fieldblock->Update();
			fieldblock2->Update();
			fieldblock3->Update();
			fieldblock4->Update();
			fieldblock5->Update();
			fieldblock6->Update();
			fieldblock7->Update();
			fieldblock8->Update();
			fieldblock9->Update();
			fieldblock10->Update();
			fieldblock11->Update();
			fieldblock12->Update();
			fieldblock13->Update();
			fieldblock14->Update();
			fieldblock15->Update();

			//敵の動き
			for (std::unique_ptr<Enemy>& enemy : enemys_) {
				enemy->Update();
			}
		}
		

#pragma endregion

		XMMATRIX matTrans;
		matTrans = XMMatrixTranslation(pos.x, pos.y, 0.0f);
		matWorld *= matTrans;

		constMapTransform->mat = matWorld * matProjection;

#pragma region  当たり判定

		if (scene == 1) {
			XMFLOAT3 posA, posB;

			//自弾リストの取得
			const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();

			posA = player->GetWorldPos();

			//自機と敵の弾の当たり判定
			for (const std::unique_ptr<Enemy>& enemy : enemys_) {
				for (const std::unique_ptr<EnemyBullet>& bullet : enemy->GetBullets()) {
					//敵弾の座標
					posB = bullet->GetWorldPos();

					//AとBの距離
					float r1 = 7.0f;	//敵のスケール
					float r2 = 1.0f;	//弾のスケール
					float r = r1 + r2;

					XMFLOAT3 dis;
					dis.x = posB.x - posA.x;
					dis.y = posB.y - posA.y;
					dis.z = posB.z - posA.z;

					if ((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z) <= (r * r)) {
						
						if (isPlayerDamege == 0 && invincible == 0) {
							bullet->isDead_ = true;
							isPlayerDamege = 1;
							player->OnCollision();
							SoundPlayWave(xAudio2.Get(), soundData5);
						}
					}
				}
			}

			for (const std::unique_ptr<Enemy>& enemy : enemys_) {
				posA = enemy->GetWorldPos();

				//自機の弾と敵の当たり判定
				for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
					//自弾の座標
					posB = bullet->GetWorldPos();

					//AとBの距離
					float r1 = 7.0f;	//敵のスケール
					float r2 = 1.0f;	//弾のスケール
					float r = r1 + r2;

					XMFLOAT3 dis;
					dis.x = posB.x - posA.x;
					dis.y = posB.y - posA.y;
					dis.z = posB.z - posA.z;


					if ((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z) <= (r * r)) {
						bullet->isDead_ = true;
						enemy->OnCollision();

						if (enemy->IsDead() == true) {
							knockDownNum++;
						}
						SoundPlayWave(xAudio2.Get(), soundData2);
					}
				}
			}
		}
		

#pragma endregion

		//ここから描画処理

		if (scene == 0) {
			// 描画前処理
			dxBasis->PreDraw();

			Object3D::PreDraw(dxBasis->GetCommandList());

			//天球
			object3d3->Draw();

			//UI->Draw();
			//UI2->Draw();

			//clearUI->Draw();
			//UIobj[0]->Draw();
			//UIobj[1]->Draw();
			//UIobj[2]->Draw();

			//UIobj[3]->Draw();
			//UIobj[4]->Draw();

			//UIobj[5]->Draw();
			//UIobj[6]->Draw();
			//UIobj[7]->Draw();
			//UIobj[8]->Draw();
			UIobj[9]->Draw();
			for (int i = 0; i < 11; i++) {
				
			}

			Object3D::PostDraw();

			//描画後処理
			dxBasis->PostDraw();
		}
		else if (scene == 1) {
#pragma region DirectX毎フレーム処理

			// 描画前処理
			dxBasis->PreDraw();

			Object3D::PreDraw(dxBasis->GetCommandList());

#pragma region  3Dモデル描画処理
			object3d3->Draw();

			floorObject->Draw();

			fieldblock->Draw();
			fieldblock2->Draw();
			fieldblock3->Draw();
			fieldblock4->Draw();
			fieldblock5->Draw();
			fieldblock6->Draw();
			fieldblock7->Draw();
			fieldblock8->Draw();
			fieldblock9->Draw();
			fieldblock10->Draw();
			fieldblock11->Draw();
			fieldblock12->Draw();
			fieldblock13->Draw();
			fieldblock14->Draw();
			fieldblock15->Draw();

			//playerObject->Draw();
			player->Draw();

			for (std::unique_ptr<Enemy>& enemy : enemys_) {
				enemy->Draw();
			}

			/*for (int i = 0; i < 3; i++) {
				enemy[i]->Draw();
			}*/
#pragma endregion

			Object3D::PostDraw();

			// パイプラインステート設定
			dxBasis->GetCommandList()->SetPipelineState(pipelineState.Get());

			// ルートシグネチャの設定コマンド
			dxBasis->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());

			// プリミティブ形状の設定コマンド
			dxBasis->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

			// 定数バッファビュー(CBV)の設定コマンド
			dxBasis->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

			//デスクリプタヒープの配列をセットするコマンド
			ID3D12DescriptorHeap* ppHeaps[] = { srvHeap.Get() };
			dxBasis->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
			//SRVヒープの先頭アドレスを取得
			D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
			//SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
			dxBasis->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

			//定数バッファビュー(CBV)の設定コマンド
			dxBasis->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

			// 頂点バッファビューの設定コマンド
			dxBasis->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

			// インデックスバッファビューの設定コマンド
			dxBasis->GetCommandList()->IASetIndexBuffer(&ibView);

			// 描画コマンド
			dxBasis->GetCommandList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0); // 全ての頂点を使って描画

			//描画後処理
			dxBasis->PostDraw();

#pragma endregion
		}
		else if (scene == 2) {
			// 描画前処理
			dxBasis->PreDraw();

			Object3D::PreDraw(dxBasis->GetCommandList());

			//天球
			object3d3->Draw();

			floorObject->Draw();

			fieldblock->Draw();
			fieldblock2->Draw();
			fieldblock3->Draw();
			fieldblock4->Draw();
			fieldblock5->Draw();
			fieldblock6->Draw();
			fieldblock7->Draw();
			fieldblock8->Draw();
			fieldblock9->Draw();
			fieldblock10->Draw();
			fieldblock11->Draw();
			fieldblock12->Draw();
			fieldblock13->Draw();
			fieldblock14->Draw();
			fieldblock15->Draw();

			for (std::unique_ptr<Enemy>& enemy : enemys_) {
				enemy->Draw();
			}

			Object3D::PostDraw();

			//描画後処理
			dxBasis->PostDraw();
		}
		else if (scene == 3) {
		// 描画前処理
		dxBasis->PreDraw();

		Object3D::PreDraw(dxBasis->GetCommandList());

		//天球
		object3d3->Draw();

		Object3D::PostDraw();

		//描画後処理
		dxBasis->PostDraw();
		}
	}

	delete model;
	delete model2;
	delete playerModel;
	delete floorModel;
	delete fieldBlock;
	delete fieldBlock2;

	delete enemyObject;
	delete enemyObject2;
	delete enemyObject3;

	delete floorObject;
	delete playerObject;
	delete object3d3;
	delete player;
	delete fieldblock;
	delete fieldblock2;
	delete fieldblock3;
	delete fieldblock4;
	delete fieldblock5;
	delete fieldblock6;
	delete fieldblock7;
	delete fieldblock8;
	delete fieldblock9;
	delete fieldblock10;
	delete fieldblock11;
	delete fieldblock12;
	delete fieldblock13;
	delete fieldblock14;
	delete fieldblock15;

	xAudio2.Reset();
	SoundUnload(&soundData1);
	SoundUnload(&soundData2);
	SoundUnload(&soundData3);
	SoundUnload(&soundData4);

	//ウィンドウクラスを登録解除
	winApp->Release();

	return 0;
}