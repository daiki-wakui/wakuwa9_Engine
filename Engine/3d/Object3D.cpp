#include "Object3D.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ID3D12Device* Object3D::sDevice = nullptr;
ID3D12GraphicsCommandList* Object3D::sCmdList = nullptr;
ComPtr<ID3D12RootSignature> Object3D::sRootsignature;
ComPtr<ID3D12PipelineState> Object3D::sPipelinestate;
XMMATRIX Object3D::sMatView{};
XMMATRIX Object3D::sMatProjection{};
XMFLOAT3 Object3D::sEye = { 0, 20, -30.0f };
XMFLOAT3 Object3D::sTarget = { 0, 10, 0 };
XMFLOAT3 Object3D::sUp = { 0, 1, 0 };

//DirectionalLight* Object3D::light = nullptr;
LightGroup* Object3D::sLightGroup = nullptr;

XMMATRIX Object3D::matBillborad = XMMatrixIdentity();
XMMATRIX Object3D::matBillboradY = XMMatrixIdentity();

void Object3D::StaticInitialize(ID3D12Device* device, int32_t window_width, int32_t window_height)
{
	// nullptrチェック
	assert(device);

	Object3D::sDevice = device;

	Model::SetDevice(device);

	// カメラ初期化
	InitializeCamera(window_width, window_height);

	// パイプライン初期化
	InitializeGraphicsPipeline();

	// モデル生成
	CreateModel();

}

void Object3D::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	// PreDrawとPostDrawがペアで呼ばれていなければエラー
	assert(Object3D::sCmdList == nullptr);

	// コマンドリストをセット
	Object3D::sCmdList = cmdList;

	// パイプラインステートの設定
	cmdList->SetPipelineState(sPipelinestate.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(sRootsignature.Get());
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Object3D::PostDraw()
{
	// コマンドリストを解除
	Object3D::sCmdList = nullptr;
}

void Object3D::SetEye(XMFLOAT3& eye)
{
	Object3D::sEye = eye;

	UpdateViewMatrix();
}

void Object3D::SetTarget(XMFLOAT3& target)
{
	Object3D::sTarget = target;

	UpdateViewMatrix();
}

void Object3D::CameraMoveVector(XMFLOAT3& move)
{
	XMFLOAT3 eye_moved = GetEye();
	XMFLOAT3 target_moved = GetTarget();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	target_moved.x += move.x;
	target_moved.y += move.y;
	target_moved.z += move.z;

	SetEye(eye_moved);
	SetTarget(target_moved);
}

void Object3D::CameraEyeMoveVector(Vector3& eye)
{
	XMFLOAT3 eye_ = GetEye();
	Vector3 VecEye_;
	VecEye_.x = eye_.x;
	//VecEye_.y = eye_.y;
	VecEye_.z = eye_.z;

	VecEye_.x += eye.x;
	//VecEye_.y += eye.y;
	VecEye_.z += eye.z;

	eye_.x = VecEye_.x;
	//eye_.y = VecEye_.y;
	eye_.z = VecEye_.z;

	SetEye(eye_);
}

void Object3D::InitializeCamera(int32_t window_width, int32_t window_height)
{
	// ビュー行列の生成
	//sMatView = XMMatrixLookAtLH(
	//	XMLoadFloat3(&sEye),
	//	XMLoadFloat3(&sTarget),
	//	XMLoadFloat3(&sUp));

	UpdateViewMatrix();


	// 透視投影による射影行列の生成
	sMatProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)window_width / window_height,
		0.1f, 1000.0f
	);
}

void Object3D::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/OBJVertexShader.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/OBJPixelShader.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 法線ベクトル(1行で書いたほうが見やすい)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV = {};
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[4] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = sDevice->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&sRootsignature));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = sRootsignature.Get();

	// グラフィックスパイプラインの生成
	result = sDevice->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&sPipelinestate));
	assert(SUCCEEDED(result));

}

void Object3D::CreateModel()
{

}

void Object3D::UpdateViewMatrix()
{
	// ビュー行列の更新
	//sMatView = XMMatrixLookAtLH(XMLoadFloat3(&sEye), XMLoadFloat3(&sTarget), XMLoadFloat3(&sUp));

	XMVECTOR eyePosition = XMLoadFloat3(&sEye);
	XMVECTOR targetPosition = XMLoadFloat3(&sTarget);
	XMVECTOR upVector = XMLoadFloat3(&sUp);

	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);

	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));

	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	XMVECTOR cameraAxisX;
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	XMVECTOR cameraAxisY;
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	XMMATRIX matCameraRot;

	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);

	sMatView = XMMatrixTranspose(matCameraRot);

	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);

	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);

	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

	sMatView.r[3] = translation;

#pragma region  全方向ビルボード行列の計算

	//ビルボード行列
	matBillborad.r[0] = cameraAxisX;
	matBillborad.r[1] = cameraAxisY;
	matBillborad.r[2] = cameraAxisZ;
	matBillborad.r[3] = XMVectorSet(0, 0, 0, 1);

#pragma region

#pragma region  Y軸ビルボード行列の計算

	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	ybillCameraAxisX = cameraAxisX;

	ybillCameraAxisY = XMVector3Normalize(upVector);
	ybillCameraAxisZ = XMVector3Cross(ybillCameraAxisX, ybillCameraAxisY);

	matBillboradY.r[0] = ybillCameraAxisX;
	matBillboradY.r[1] = ybillCameraAxisY;
	matBillboradY.r[2] = ybillCameraAxisZ;
	matBillboradY.r[3] = XMVectorSet(0, 0, 0, 1);


#pragma region
}

bool Object3D::Initialize()
{
	// nullptrチェック
	assert(sDevice);

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff);

	HRESULT result;

	// 定数バッファの生成
	result = sDevice->CreateCommittedResource(
		&heapProps, // アップロード可能
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffB0_));
	assert(SUCCEEDED(result));





	return true;
}

void Object3D::Update(bool billborad)
{
	HRESULT result;
	XMMATRIX matScale, matRot, matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation_.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation_.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation_.y));
	matTrans = XMMatrixTranslation(position_.x, position_.y, position_.z);

	// ワールド行列の合成
	matWorld_ = XMMatrixIdentity(); // 変形をリセット
	matWorld_ *= matScale; // ワールド行列にスケーリングを反映
	matWorld_ *= matRot; // ワールド行列に回転を反映
	matWorld_ *= matTrans; // ワールド行列に平行移動を反映

	if (billborad) {
		matWorld_ *= matBillboradY;
	}

	// 親オブジェクトがあれば
	if (parent_ != nullptr) {
		// 親オブジェクトのワールド行列を掛ける
		matWorld_ *= parent_->matWorld_;
	}

	// 定数バッファへデータ転送
	ConstBufferDataB0* constMap = nullptr;
	result = constBuffB0_->Map(0, nullptr, (void**)&constMap);
	//constMap->color = color;
	constMap->viewproj = sMatView * sMatProjection;
	constMap->world = matWorld_;
	constMap->cameraPos = GetEye();
	constBuffB0_->Unmap(0, nullptr);


}

void Object3D::Draw()
{
	// nullptrチェック
	assert(sDevice);
	assert(Object3D::sCmdList);

	//モデルの紐づけがない場合描画しない
	if (model_ == nullptr) {
		return;
	}

	// 定数バッファビューをセット
	sCmdList->SetGraphicsRootConstantBufferView(0, constBuffB0_->GetGPUVirtualAddress());
	
	//ライトの描画
	//light->Draw(cmdList, 3);
	sLightGroup->Draw(sCmdList, 3);

	model_->Draw(sCmdList, 1);
}

void Object3D::SetCamera(const XMFLOAT3& eye,const XMFLOAT3& terget)
{
	XMFLOAT3 setCamera_e = eye;
	XMFLOAT3 setCamera_t = terget;

	SetEye(setCamera_e);
	SetTarget(setCamera_t);
}

XMFLOAT3 Object3D::Screen()
{
	XMMATRIX view = sMatView;
	XMMATRIX proj = sMatProjection;

	float w = 1280 / 2.0f;
	float h = 720 / 2.0f;

	XMMATRIX viewport = {
		w, 0,0,0,
		0,-h,0,0,
		0, 0,1,0,
		w, h,0,1
	};

	XMFLOAT3 pos = position_;

	pos = VTransform(pos, view);
	pos = VTransform(pos, proj);

	pos.x /= pos.z;
	pos.y /= pos.z;
	pos.z /= pos.z;

	pos = VTransform(pos, viewport);

	return pos;
}

XMFLOAT3 Object3D::VTransform(XMFLOAT3 InV, XMMATRIX InM)
{
	XMFLOAT3 result;

	result.x = InV.x * InM.r[0].m128_f32[0] + InV.y * InM.r[1].m128_f32[0] + InV.z * InM.r[2].m128_f32[0] + InM.r[3].m128_f32[0];
	result.y = InV.x * InM.r[0].m128_f32[1] + InV.y * InM.r[1].m128_f32[1] + InV.z * InM.r[2].m128_f32[1] + InM.r[3].m128_f32[1];
	result.z = InV.x * InM.r[0].m128_f32[2] + InV.y * InM.r[1].m128_f32[2] + InV.z * InM.r[2].m128_f32[2] + InM.r[3].m128_f32[2];

	return result;
}

