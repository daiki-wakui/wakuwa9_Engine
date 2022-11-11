#include "WindowsApp.h"
#include "KeyBoard.h"
#include "DirectXBasis.h"
#include <memory>
#include <string>

#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//windowsAPIの生成クラス
	std::unique_ptr<WindowsApp> windows;
	WindowsApp* win = new WindowsApp();

	//keyboradクラスの生成
	std::unique_ptr<KeyBoard> keyboard;
	KeyBoard* input_ = new KeyBoard();

	//DirectXの基盤生成クラス
	std::unique_ptr<DirectXBasis> DirectX;
	DirectXBasis* dxBasis = new DirectXBasis();

	//windowsAPI初期化
	win->Initalize();
	windows.reset(win);
	MSG msg{};

	//DirectX初期化
	dxBasis->Initialize(win);
	DirectX.reset(dxBasis);

	//keyborad初期化
	input_->Initialize(win->GetHInstancee(), win->GetHwnd());
	keyboard.reset(input_);

#pragma region  描画初期化処理
//
//#pragma region  頂点データ
//
//	// 頂点データ
//	XMFLOAT3 vertices[] = {
//	{ -0.5f, -0.5f, 0.0f }, // 左下
//	{ -0.5f, +0.5f, 0.0f }, // 左上
//	{ +0.5f, -0.5f, 0.0f }, // 右下
//	};
//	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
//	UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));
//
//#pragma endregion
//
//#pragma region  頂点バッファの設定
//	// 頂点バッファの設定
//	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
//	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
//	// リソース設定
//	D3D12_RESOURCE_DESC resDesc{};
//	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
//	resDesc.Height = 1;
//	resDesc.DepthOrArraySize = 1;
//	resDesc.MipLevels = 1;
//	resDesc.SampleDesc.Count = 1;
//	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//#pragma endregion
//
//#pragma region  頂点バッファの生成
//
//	// 頂点バッファの生成
//	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;
//	result = device->CreateCommittedResource(
//		&heapProp, // ヒープ設定
//		D3D12_HEAP_FLAG_NONE,
//		&resDesc, // リソース設定
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&vertBuff));
//	assert(SUCCEEDED(result));
//
//#pragma endregion
//
//#pragma region  頂点バッファへのデータ転送
//
//	//GPU上のバッファに対応した仮想メモリ
//	XMFLOAT3* vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
//	assert(SUCCEEDED(result));
//	//全頂点に対して
//	for (int i = 0; i < _countof(vertices); i++) {
//		vertMap[i] = vertices[i];
//	}
//	//繋がりを解除
//	vertBuff->Unmap(0, nullptr);
//
//#pragma endregion
//
//#pragma region  頂点バッファビューの生成
//
//	// 頂点バッファビューの作成
//	D3D12_VERTEX_BUFFER_VIEW vbView{};
//	// GPU仮想アドレス
//	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
//	// 頂点バッファのサイズ
//	vbView.SizeInBytes = sizeVB;
//	// 頂点1つ分のデータサイズ
//	vbView.StrideInBytes = sizeof(XMFLOAT3);
//
//#pragma endregion
//
//#pragma region  頂点シェーダーファイルの読み込み
//
//	ID3DBlob* vsBlob = nullptr; // 頂点シェーダオブジェクト
//	ID3DBlob* psBlob = nullptr; // ピクセルシェーダオブジェクト
//	ID3DBlob* errorBlob = nullptr; // エラーオブジェクト
//	// 頂点シェーダの読み込みとコンパイル
//	result = D3DCompileFromFile(
//		L"BasicVShader.hlsl", // シェーダファイル名
//		nullptr,
//		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
//		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
//		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
//		0,
//		&vsBlob, &errorBlob);
//
//	// エラーなら
//	if (FAILED(result)) {
//		// errorBlobからエラー内容をstring型にコピー
//		std::string error;
//		error.resize(errorBlob->GetBufferSize());
//		std::copy_n((char*)errorBlob->GetBufferPointer(),
//			errorBlob->GetBufferSize(),
//			error.begin());
//		error += "\n";
//		// エラー内容を出力ウィンドウに表示
//		OutputDebugStringA(error.c_str());
//		assert(0);
//	}
//
//#pragma endregion
//
//#pragma region  ピクセルシェーダファイルの読み込み
//
//	// ピクセルシェーダの読み込みとコンパイル
//	result = D3DCompileFromFile(
//		L"BasicPShader.hlsl", // シェーダファイル名
//		nullptr,
//		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
//		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
//		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
//		0,
//		&psBlob, &errorBlob);
//
//	// エラーなら
//	if (FAILED(result)) {
//		// errorBlobからエラー内容をstring型にコピー
//		std::string error;
//		error.resize(errorBlob->GetBufferSize());
//		std::copy_n((char*)errorBlob->GetBufferPointer(),
//			errorBlob->GetBufferSize(),
//			error.begin());
//		error += "\n";
//		// エラー内容を出力ウィンドウに表示
//		OutputDebugStringA(error.c_str());
//		assert(0);
//	}
//
//#pragma endregion
//
//#pragma region  頂点レイアウト
//
//	// 頂点レイアウト
//	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
//	{
//	"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
//	D3D12_APPEND_ALIGNED_ELEMENT,
//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
//	}, // (1行で書いたほうが見やすい)
//	};
//
//#pragma endregion
//
//#pragma region  グラフィックスパイプライン設定
//
//	// グラフィックスパイプライン設定
//	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
//
//	// シェーダーの設定
//	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
//	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
//	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
//	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();
//
//	// サンプルマスクの設定
//	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
//
//	// ラスタライザの設定
//	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // カリングしない
//	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
//	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に
//
//	// ブレンドステート
//	pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
//		= D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA全てのチャンネルを描画
//
//	// 頂点レイアウトの設定
//	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
//	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
//
//	// 図形の形状設定
//	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
//
//	// その他の設定
//	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
//	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
//	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング
//
//	// ルートシグネチャ
//	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
//	// ルートシグネチャの設定
//	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
//	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
//	// ルートシグネチャのシリアライズ
//	ID3DBlob* rootSigBlob = nullptr;
//	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
//		&rootSigBlob, &errorBlob);
//	assert(SUCCEEDED(result));
//	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
//		IID_PPV_ARGS(&rootSignature));
//	assert(SUCCEEDED(result));
//	rootSigBlob->Release();
//	// パイプラインにルートシグネチャをセット
//	pipelineDesc.pRootSignature = rootSignature.Get();
//
//	//パイプラインステートの生成
//	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState = nullptr;
//	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
//	assert(SUCCEEDED(result));
//
//#pragma endregion
//
#pragma endregion

	//ゲームループ
	while (true) {
		//×ボタンで終了メッセージがきたら
		if (win->gameloopExit(msg) == true) {
			break;	//ゲームループ終了
		}

		//keyborad更新処理
		input_->Update();

#pragma region DirectX毎フレーム処理

		//描画前処理
		dxBasis->PreDraw();

#pragma region  パイプラインステート設定
//
//		// パイプラインステートとルートシグネチャの設定コマンド
//		commandList->SetPipelineState(pipelineState.Get());
//		commandList->SetGraphicsRootSignature(rootSignature.Get());
//
//#pragma endregion
//
//#pragma region  プリニティブ形状
//
//		// プリミティブ形状の設定コマンド
//		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト
//
//#pragma endregion
//
//#pragma region  頂点バッファビューの設定
//
//		// 頂点バッファビューの設定コマンド
//		commandList->IASetVertexBuffers(0, 1, &vbView);
//
//#pragma endregion
//
//#pragma region  描画コマンド
//
//		// 描画コマンド
//		commandList->DrawInstanced(_countof(vertices), 1, 0, 0); // 全ての頂点を使って描画
//
//#pragma endregion
//
#pragma endregion

		//描画後処理
		dxBasis->PostDraw();

#pragma endregion

	}

	//ウィンドウクラスを登録解除
	win->Release();

	return 0;
}