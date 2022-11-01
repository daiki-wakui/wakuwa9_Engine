#include "WindowsApp.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//windowsAPIの生成クラス
	WindowsApp* win = nullptr;
	win = new WindowsApp();

	////windowsAPI初期化
	win->Initalize();
	MSG msg{};

#pragma region  DirectX初期化処理

#pragma endregion

	//ゲームループ
	while (true) {
		//×ボタンで終了メッセージがきたら
		if (win->gameloopExit(msg) == true) {
			break;	//ゲームループ終了
		}
#pragma region DirectX毎フレーム処理

#pragma endregion

	}

	//ウィンドウクラスを登録解除
	win->Release();
	//解放
	delete win;
	return 0;
}