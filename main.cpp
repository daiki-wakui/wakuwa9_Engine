#include <windows.h>
#include "WindowsApp.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//OutputDebugStringA("Hello,DirectX!!\n");

	WindowsApp* win = nullptr;

	win = new WindowsApp();
	win->Initalize();
	MSG msg{};

#pragma region  DirectX初期化処理

#pragma endregion

	//ゲームループ
	while (true) {
		
		if (win->gameloopExit(msg) == true) {
			break;
		}
#pragma region DirectX毎フレーム処理

#pragma endregion

	}


	win->Release();

	delete win;
	return 0;
}