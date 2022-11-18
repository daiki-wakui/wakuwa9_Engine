#include "WindowsApp.h"
#include "KeyBoard.h"
#include "DirectXBasis.h"
#include "Sprite.h"
#include "SpriteBasis.h"
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

	//DirectXの基盤生成クラス
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

	//スプライト共通部の初期化
	spBasis->Initialize(dxBasis);
	SpBasis.reset(spBasis);

#pragma endregion	

#pragma region  描画初期化処理

	Sprite* sprite = new Sprite();
	sprite->Initialize(spBasis);

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



#pragma endregion

#pragma region DirectX毎フレーム描画処理
		// 描画前処理
		dxBasis->PreDraw();



		//描画後処理
		dxBasis->PostDraw();
#pragma endregion
	}
	//ウィンドウクラスを登録解除
	winApp->Release();

	return 0;
}