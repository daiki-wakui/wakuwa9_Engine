#include "WindowsApp.h"
#include "KeyBoard.h"
#include "DirectXBasis.h"
#include "Object3D.h"
#include "Model.h"
#include "SpriteB.h"

#include <memory>
#include <string>
#include <DirectXTex.h>

#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")


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

	//DirectX初期化
	dxBasis->Initialize(winApp);
	DirectX.reset(dxBasis);

	// スプライト静的初期化
	SpriteB::StaticInitialize(dxBasis->GetDevice(), winApp->GetWindowWidth(), winApp->GetWindowHeight());

	Object3D::StaticInitialize(dxBasis->GetDevice(), winApp->GetWindowWidth(), winApp->GetWindowHeight());

	//keyborad初期化
	input_->Initialize(winApp->GetHInstancee(), winApp->GetHwnd());
	keyboard.reset(input_);

	XMFLOAT2 pos = { 0,0 };

	SpriteB::LoadTexture(0, L"Resources/test.png");

	SpriteB* sp1 = SpriteB::Create(0, { 0,0 });

	//OBJからモデルを読み込む
	Model* model = Model::LoadFromObj("boss");
	Model* model2 = Model::LoadFromObj("world");

	//3Dオブジェクト生成
	Object3D* object3d = Object3D::Create(5.0f);
	Object3D* object3d2 = Object3D::Create(5.0f);
	Object3D* object3d3 = Object3D::Create(100.0f);

	//3Dオブジェクトに3Dモデルを紐づけ
	object3d->SetModel(model);
	object3d2->SetModel(model);
	object3d3->SetModel(model2);

	object3d->SetPosition({ -20,0,+5 });
	object3d2->SetPosition({ +20,0,+5 });

	//ゲームループ
	while (true) {
		//×ボタンで終了メッセージがきたら
		if (winApp->gameloopExit(msg) == true) {
			break;	//ゲームループ終了
		}
		object3d->Update();
		object3d2->Update();
		object3d3->Update();

		//keyborad更新処理
		input_->Update();

		XMFLOAT3 pos3d;
		XMFLOAT3 pos3d2;

		pos3d = object3d->GetRotation();
		pos3d2 = object3d2->GetPosition();

		if (input_->keyPush(DIK_D)) {
			pos3d.y++;
		}
		if (input_->keyPush(DIK_A)) {
			pos3d.y--;
		}
		if (input_->keyPush(DIK_S)) {
			pos3d.x--;
		}
		if (input_->keyPush(DIK_W)) {
			pos3d.x++;
		}

		if (input_->keyPush(DIK_RIGHT)) {
			pos3d2.x++;
		}
		if (input_->keyPush(DIK_LEFT)) {
			pos3d2.x--;
		}
		if (input_->keyPush(DIK_UP)) {
			pos3d2.y++;
		}
		if (input_->keyPush(DIK_DOWN)) {
			pos3d2.y--;
		}

		object3d->SetRotation(pos3d);
		object3d2->SetPosition(pos3d2);

	/*	XMMATRIX matTrans;
		matTrans = XMMatrixTranslation(pos.x, pos.y, 0.0f);
		matWorld *= matTrans;

		constMapTransform->mat = matWorld * matProjection;*/

#pragma region DirectX毎フレーム処理

		// 描画前処理
		dxBasis->PreDraw();

		Object3D::PreDraw(dxBasis->GetCommandList());

		object3d->Draw();
		object3d2->Draw();
		object3d3->Draw();

		Object3D::PostDraw();

		SpriteB::PreDraw(dxBasis->GetCommandList());

		sp1->Draw();

		SpriteB::PostDraw();

		//// パイプラインステート設定
		//dxBasis->GetCommandList()->SetPipelineState(pipelineState.Get());

		//// ルートシグネチャの設定コマンド
		//dxBasis->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());

		//// プリミティブ形状の設定コマンド
		//dxBasis->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

		//// 定数バッファビュー(CBV)の設定コマンド
		//dxBasis->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

		////デスクリプタヒープの配列をセットするコマンド
		//ID3D12DescriptorHeap* ppHeaps[] = { srvHeap.Get()};
		//dxBasis->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		////SRVヒープの先頭アドレスを取得
		//D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
		////SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
		//dxBasis->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

		////定数バッファビュー(CBV)の設定コマンド
		//dxBasis->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

		//// 頂点バッファビューの設定コマンド
		//dxBasis->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

		//// インデックスバッファビューの設定コマンド
		//dxBasis->GetCommandList()->IASetIndexBuffer(&ibView);

		//// 描画コマンド
		//dxBasis->GetCommandList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0); // 全ての頂点を使って描画


		

		//描画後処理
		dxBasis->PostDraw();


		
#pragma endregion

	}

	delete model;
	delete object3d;
	delete object3d2;
	delete object3d3;
	//ウィンドウクラスを登録解除
	winApp->Release();

	return 0;
}
