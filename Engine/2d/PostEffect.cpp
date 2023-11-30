#include "PostEffect.h"
#include <d3dx12.h>
#include <cassert>
#include <algorithm>

#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include "MyRandom.h"

using namespace DirectX;

const float PostEffect::sCLEAR_COLOR[4] = { 0.1f,0.1f,0.5f,0.0f };

void PostEffect::Initialize(int32_t num)
{
	HRESULT result;

	VertexData();
	IndexData();

	//テクスチャバッファ設定
	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//テクスチャリソース設定
	D3D12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		winApp_->GetWindowWidth(),
		(UINT)winApp_->GetWindowHeight(),
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	CD3DX12_CLEAR_VALUE clearValue;

	clearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	for (int32_t i = 0; i < 4; i++) {
		clearValue.Color[i] = sCLEAR_COLOR[i];
	}

	//テクスチャバッファの生成
	for (int32_t i = 0; i < 2; i++) {
		result = spBasis_->GetDxBasis()->GetDevice()->CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&texresDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&clearValue,
			IID_PPV_ARGS(&texBuff_[i]));

		assert(SUCCEEDED(result));


		//イメージデータ転送
		const UINT pixelCount = winApp_->GetWindowWidth() * winApp_->GetWindowHeight();

		const UINT rowPitch = sizeof(UINT) * winApp_->GetWindowHeight();

		const UINT deptPitch = rowPitch * winApp_->GetWindowHeight();

		UINT* img = new UINT[pixelCount];

		for (UINT j = 0; j < pixelCount; j++) {
			img[j] = 0xff0000ff;
		}

		result = texBuff_[i]->WriteToSubresource(0, nullptr, img, rowPitch, deptPitch);
		assert(SUCCEEDED(result));
		delete[] img;
	}

	//SRV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 2;

	//SRV用デスクリプタヒープ生成
	result = spBasis_->GetDxBasis()->GetDevice()->CreateDescriptorHeap(
		&srvDescHeapDesc,
		IID_PPV_ARGS(&descHeapSRV_));

	assert(SUCCEEDED(result));


	//SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//設定構造体
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	for (int32_t i = 0; i < 2; i++) {
		//デスクリプタヒープにSRV作成
		spBasis_->GetDxBasis()->GetDevice()->CreateShaderResourceView(
			texBuff_[i].Get(),
			&srvDesc,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				descHeapSRV_->GetCPUDescriptorHandleForHeapStart(), i,
				spBasis_->GetDxBasis()->GetDevice()->GetDescriptorHandleIncrementSize(
					D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV))
		);
	}

	//RTV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 2;

	//RTV用デスクリプタヒープ生成
	result = spBasis_->GetDxBasis()->GetDevice()->CreateDescriptorHeap(
		&rtvDescHeapDesc,
		IID_PPV_ARGS(&descHeapRTV));

	assert(SUCCEEDED(result));


	//レンダーターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};

	//シェーダーの計算結果をSRGBに変換して書き込む
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	for (int32_t i = 0; i < 2; i++) {
		//デスクリプタヒープにRTV生成
		spBasis_->GetDxBasis()->GetDevice()->CreateRenderTargetView(texBuff_[i].Get(),
			nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				descHeapRTV->GetCPUDescriptorHandleForHeapStart(),i,
				spBasis_->GetDxBasis()->GetDevice()->GetDescriptorHandleIncrementSize(
					D3D12_DESCRIPTOR_HEAP_TYPE_RTV))
			
		);
	}

	//深度バッファリソース設定
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = winApp_->GetWindowWidth();
	depthResourceDesc.Height = winApp_->GetWindowHeight();
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;	//デプスステンシル

	// 深度値用ヒーププロパティ
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	//深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;	//深度値1.0f(最大値)
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット

	//深度バッファの生成
	result = spBasis_->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	//深度値書き込みに使用
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff_));

	//DSV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.NumDescriptors = 1;

	//DRV用デスクリプタヒープ生成
	result = spBasis_->GetDxBasis()->GetDevice()->CreateDescriptorHeap(
		&dsvHeapDesc, IID_PPV_ARGS(&descHeapDSV_));

	assert(SUCCEEDED(result));

	//深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	spBasis_->GetDxBasis()->GetDevice()->CreateDepthStencilView(
		depthBuff_.Get(),
		&dsvDesc,
		descHeapDSV_->GetCPUDescriptorHandleForHeapStart());

	Crate();

	//パイプライン生成
	CreateGraphicsPipelineState(num);
}

void PostEffect::VertexData()
{

	// 頂点データ
	vertices_[LB] = { {   0.0f, (float)winApp_->GetWindowHeight(), 0.0f},{0.0f , 1.0f}};
	vertices_[LT] = { {   0.0f, 0.0f, 0.0f }, { 0.0f , 0.0f } };
	vertices_[RB] = { { (float)winApp_->GetWindowWidth(), (float)winApp_->GetWindowHeight(), 0.0f},{1.0f , 1.0f}};
	vertices_[RT] = { { (float)winApp_->GetWindowWidth(),   0.0f, 0.0f },{ 1.0f , 0.0f } };

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * _countof(vertices_));

	vertexSize_ = _countof(vertices_);

	// 頂点バッファの設定
	heapProp_.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
	// リソース設定
	resDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc_.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc_.Height = 1;
	resDesc_.DepthOrArraySize = 1;
	resDesc_.MipLevels = 1;
	resDesc_.SampleDesc.Count = 1;
	resDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


	// 頂点バッファの生成

	result_ = spBasis_->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&heapProp_, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc_, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result_));

#pragma region  頂点バッファへのデータ転送

	//GPU上のバッファに対応した仮想メモリ
	Vertex* vertMap = nullptr;
	result_ = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result_));
	//全頂点に対して
	for (int32_t i = 0; i < _countof(vertices_); i++) {
		vertMap[i] = vertices_[i];
	}
	//繋がりを解除
	vertBuff_->Unmap(0, nullptr);

#pragma endregion

#pragma region  頂点バッファビューの生成

	// GPU仮想アドレス
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView_.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	vbView_.StrideInBytes = sizeof(vertices_[0]);

#pragma endregion
}

void PostEffect::IndexData()
{
	//インデックスデータ
	uint16_t indices[] = {
		0 , 1 , 2,	//三角形1つ目
		1 , 2 , 3,	//三角形2つ目
	};

	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	indexSize_ = _countof(indices);

	//リソース設定
	resDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc_.Width = sizeIB;	//インデックス情報が入る分のサイズ
	resDesc_.Height = 1;
	resDesc_.DepthOrArraySize = 1;
	resDesc_.MipLevels = 1;
	resDesc_.SampleDesc.Count = 1;
	resDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	heapProp_.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

	//インデックスバッファの生成
	result_ = spBasis_->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&heapProp_,
		D3D12_HEAP_FLAG_NONE,
		&resDesc_,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff_));

	//インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result_ = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	//全インデックスに対して
	for (int32_t i = 0; i < _countof(indices); i++) {
		indexMap[i] = indices[i];
	}
	//マッピング解除
	indexBuff_->Unmap(0, nullptr);

	//インデックスバッファビューの生成
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	ibView_.SizeInBytes = sizeIB;
}

void PostEffect::Crate()
{
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


	//color
	//定数バッファの生成
	result_ = spBasis_->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&cbHeapProp,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial_));
	assert(SUCCEEDED(result_));

	//mat
	//定数バッファの生成
	result_ = spBasis_->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&cbMatHeapProp,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbMatResourceDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform_));
	assert(SUCCEEDED(result_));

	//定数バッファのマッピング(color)
	result_ = constBuffMaterial_->Map(0, nullptr, (void**)&constMapMaterial);	//マッピング
	assert(SUCCEEDED(result_));

	//定数バッファのマッピング(mat)
	result_ = constBuffTransform_->Map(0, nullptr, (void**)&constMapTransform_);
	assert(SUCCEEDED(result_));

	//値を書きこんで自動転送
	constMapMaterial->color = XMFLOAT4(1, 1, 1, 1);	//色変更
	
	//左上を原点に設定
	matProjection_ =
		XMMatrixOrthographicOffCenterLH(
			0.0f, (float)winApp_->GetWindowWidth(),
			(float)winApp_->GetWindowHeight(), 0.0f,
			0.0f, 1.0f);

	matWorld_ = XMMatrixIdentity();

	matScale_ = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	matWorld_ *= matScale_;

	matRot_ = XMMatrixIdentity();
	matRot_ *= XMMatrixRotationZ(XMConvertToRadians(0.0f));	//Z 0度回転
	matRot_ *= XMMatrixRotationX(XMConvertToRadians(0.0f));	//X 0度回転
	matRot_ *= XMMatrixRotationY(XMConvertToRadians(0.0f));	//Y 0度回転
	matWorld_ *= matRot_;

	matTrans_ = XMMatrixTranslation(0.0f,0.0f, 0.0f);
	matWorld_ *= matTrans_;

	constMapTransform_->mat = matWorld_ * matProjection_;
}

PostEffect::PostEffect()
{

}

void PostEffect::Update(Player* player)
{
	if (player->GetIsHit()) {
		isEffect_ = true;
		frame_ = 30;
	}

	if (isEffect_) {
		frame_--;
		noiseTimer_--;

		if (frame_ <= 30 && noiseTimer_ < 0) {

			constMapMaterial->power = MyRandom::GetFloatRandom(0.1f, 1.0f);

			if (player->GetHP() <= 1) {
				constMapMaterial->shiftPower = MyRandom::GetFloatRandom(-0.1f, 0.1f);
				noiseTimer_ = 2;
			}
			else {
				constMapMaterial->shiftPower = MyRandom::GetFloatRandom(-0.02f, 0.02f);
				noiseTimer_ = 1;
			}
			
			player->SetIsHit(false);
		}

		if (frame_ <= 0) {

			isEffect_ = false;
			constMapMaterial->power = 0;
			constMapMaterial->shiftPower = 0;
			frame_ = 60;
			
		}
	}
	else {
		constMapMaterial->power = 0;
		constMapMaterial->shiftPower = 0;
	}
	
}

void PostEffect::CreateGraphicsPipelineState(int32_t num)
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト

	if (num == 0) {
		// 頂点シェーダの読み込みとコンパイル
		result = D3DCompileFromFile(
			L"Resources/shaders/PostEffectVS.hlsl", // シェーダファイル名
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

		// ピクセルシェーダの読み込みとコンパイル
		result = D3DCompileFromFile(
			L"Resources/shaders/PostEffectPS.hlsl", // シェーダファイル名
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
	}
	else if(num == 1){
		// 頂点シェーダの読み込みとコンパイル
		result = D3DCompileFromFile(
			L"Resources/shaders/GaussianVS.hlsl", // シェーダファイル名
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

		// ピクセルシェーダの読み込みとコンパイル
		result = D3DCompileFromFile(
			L"Resources/shaders/GaussianPS.hlsl", // シェーダファイル名
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
	}
	else {
		// 頂点シェーダの読み込みとコンパイル
		result = D3DCompileFromFile(
			L"Resources/shaders/HighLumiVS.hlsl", // シェーダファイル名
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

		// ピクセルシェーダの読み込みとコンパイル
		result = D3DCompileFromFile(
			L"Resources/shaders/HighLumiPS.hlsl", // シェーダファイル名
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
	}
	

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{	//xy座標
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



	// ブレンドステート
	//レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	//RGBA全てのチャンネルを描画

	//半透明合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;


	//共通設定(アルファ値)
	blenddesc.BlendEnable = true;	//ブレンドを有効
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;	//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を0%使う

	//深度バッファのフォーマット
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// その他の設定
	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	//デスクリプタレンジの設定
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;	//テクスチャレジスタ0番
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//デスクリプタレンジ2の設定
	D3D12_DESCRIPTOR_RANGE descriptorRange2{};

	//デスクリプタレンジ2の設定
	descriptorRange2.NumDescriptors = 1;
	descriptorRange2.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange2.BaseShaderRegister = 1;	//テクスチャレジスタ1番
	descriptorRange2.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	//ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams[4] = {};
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
	//テクスチャレジスタ1番
	rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[3].DescriptorTable.pDescriptorRanges = &descriptorRange2;
	rootParams[3].DescriptorTable.NumDescriptorRanges = 1;
	rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};

	//テクスチャからどのように色を取り出すかの設定
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	//横繰り返し
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	//縦繰り返し
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	//奥行繰り返し
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;	//ミニマップ最大値
	samplerDesc.MinLOD = 0.0f;	//ミニマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	// ルートシグネチャ
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

	result = spBasis_->GetDxBasis()->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));

	rootSigBlob->Release();

	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature.Get();

	//パイプラインステートの生成
	result = spBasis_->GetDxBasis()->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));


}

void PostEffect::Draw()
{

	// パイプラインステート設定
	spBasis_->GetDxBasis()->GetCommandList()->SetPipelineState(pipelineState.Get());

	// ルートシグネチャの設定コマンド
	spBasis_->GetDxBasis()->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());

	//プリニティブ形状を設定
	spBasis_->GetDxBasis()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // 三角形リスト

	// 定数バッファビュー(CBV)の設定コマンド
	spBasis_->GetDxBasis()->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial_->GetGPUVirtualAddress());

	//デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV_.Get() };
	spBasis_->GetDxBasis()->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//SRVヒープの先頭アドレスを取得

	//SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	spBasis_->GetDxBasis()->GetCommandList()->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeapSRV_->GetGPUDescriptorHandleForHeapStart(),0,
			spBasis_->GetDxBasis()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV))
		);

	spBasis_->GetDxBasis()->GetCommandList()->SetGraphicsRootDescriptorTable(3,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeapSRV_->GetGPUDescriptorHandleForHeapStart(), 1,
			spBasis_->GetDxBasis()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV))
	);
	

	//定数バッファビュー(CBV)の設定コマンド
	spBasis_->GetDxBasis()->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform_->GetGPUVirtualAddress());

	// 頂点バッファビューの設定コマンド
	spBasis_->GetDxBasis()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);

	// インデックスバッファビューの設定コマンド
	spBasis_->GetDxBasis()->GetCommandList()->IASetIndexBuffer(&ibView_);

	// 描画コマンド
	spBasis_->GetDxBasis()->GetCommandList()->DrawIndexedInstanced(indexSize_, 1, 0, 0, 0); // 全ての頂点を使って描画
}

void PostEffect::PreDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	for (int32_t i = 0; i < 2; i++) {
		// 1.リソースバリアで書き込み可能に変更
		barrierDesc_.Transition.pResource = texBuff_[i].Get(); // バックバッファを指定
		barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE; // 表示状態から
		barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態へ
		cmdList->ResourceBarrier(1, &barrierDesc_);
	}

	//リソースバリアを変更
	//レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHs[2];

	for (int32_t i = 0; i < 2; i++) {
		rtvHs[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i,
			spBasis_->GetDxBasis()->GetDevice()->GetDescriptorHandleIncrementSize(
				D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		);
	}


	//深度ステンシルビュー用ディスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV_->GetCPUDescriptorHandleForHeapStart();

	//レンダーターゲットをセット
	cmdList->OMSetRenderTargets(2, rtvHs, false, &dsvH);

	// ビューポート設定コマンド

	CD3DX12_VIEWPORT viewports[2];

	// シザー矩形
	D3D12_RECT scissorRects[2] = {};

	for (int32_t i = 0; i < 2; i++) {
		viewports[i].TopLeftX = 0.0f;
		viewports[i].TopLeftY = 0.0f;
		viewports[i].Width = (float)winApp_->GetWindowWidth();
		viewports[i].Height = (float)winApp_->GetWindowHeight();
		viewports[i].MinDepth = 0.0f;
		viewports[i].MaxDepth = 1.0f;

		scissorRects[i].left = 0; // 切り抜き座標左
		scissorRects[i].right = scissorRects[i].left + winApp_->GetWindowWidth(); // 切り抜き座標右
		scissorRects[i].top = 0; // 切り抜き座標上
		scissorRects[i].bottom = scissorRects[i].top + winApp_->GetWindowHeight(); // 切り抜き座標下
	}
	// ビューポート設定コマンドを、コマンドリストに積む
	cmdList->RSSetViewports(2, viewports);
	
	// シザー矩形設定コマンドを、コマンドリストに積む
	cmdList->RSSetScissorRects(2, scissorRects);


	//ビューポートの設定

	for (int32_t i = 0; i < 2; i++) {
		//全画面クリア
		cmdList->ClearRenderTargetView(rtvHs[i], sCLEAR_COLOR, 0, nullptr);
	}
	

	//深度バッファクリア
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void PostEffect::PostDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	//リソースバリアを変更
	for (int32_t i = 0; i < 2; i++) {
		barrierDesc_.Transition.pResource = texBuff_[i].Get(); // バックバッファを指定
		barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態から
		barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE; // 表示状態へ
		cmdList->ResourceBarrier(1, &barrierDesc_);
	}
}
