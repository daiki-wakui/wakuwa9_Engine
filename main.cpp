#include "WindowsApp.h"
#include "KeyBoard.h"
#include "DirectXBasis.h"
#include "Sprite.h"
#include "SpriteBasis.h"
#include <memory>
#include <DirectXTex.h>

#include <string>
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

	//Spriteの基盤生成クラス
	std::unique_ptr<SpriteBasis> SpBasis;
	SpriteBasis* spBasis = new SpriteBasis();


#pragma region  基盤部分の初期化

	//windowsAPI初期化
	winApp->Initalize();
	windows.reset(winApp);
	MSG msg{};

	//DirectX初期化
	dxBasis->Initialize(winApp);
	DirectX.reset(dxBasis);

	//keyborad初期化
	input_->Initialize(winApp->GetHInstancee(), winApp->GetHwnd());
	keyboard.reset(input_);

	spBasis->Initialize(dxBasis,0);
	SpBasis.reset(spBasis);

#pragma endregion	

#pragma region  描画初期化処理
	
	HRESULT result;

	Sprite* sprite = new Sprite();
	sprite->Initialize(spBasis, winApp);

	/*Sprite* sprite2 = new Sprite();
	sprite2->Initialize(spBasis, winApp);*/

	sprite->Create(150,150);
	//sprite2->Create();


	XMFLOAT2 posS = { 0,0 };
	XMFLOAT2 sizeS = { 0,0 };
	float speed = 1;
	float angle = 0;
#pragma endregion

	//ゲームループ
	while (true) {
		//×ボタンで終了メッセージがきたら
		if (winApp->gameloopExit(msg) == true) {
			break;	//ゲームループ終了
		}

		//keyborad更新処理
		input_->Update();

#pragma region  DirectX毎フレーム更新処理
		

		sizeS = sprite->GetSize();

		posS.x = 0;
		posS.y = 0;

		angle = 0;

		angle += 1;

		if (input_->keyPush(DIK_D)) {
			//posS.x += speed;
			sizeS.x++;
		}
		else if (input_->keyPush(DIK_A)) {
			//posS.x -= speed;
			sizeS.x--;
		}
		if (input_->keyPush(DIK_W)) {
			//posS.y -= speed;
			sizeS.y++;
		}
		else if (input_->keyPush(DIK_S)) {
			//posS.y += speed;
			sizeS.y--;
		}

		

		//sprite
		//sprite->SetPosition(posS);
		sprite->SetSize(sizeS);
		//sprite->SetRotation(angle);
		//sprite2->SetRotation(angle);

		//sprite2->Update();
		sprite->Update();

#pragma endregion

#pragma region DirectX毎フレーム描画処理
		// 描画前処理
		dxBasis->PreDraw();

		sprite->Draw();
		//sprite2->Draw();

		//描画後処理
		dxBasis->PostDraw();
#pragma endregion
	}
	//ウィンドウクラスを登録解除
	winApp->Release();
	delete sprite;
	return 0;
}