#include <windows.h>
#include "WindowsApp.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//OutputDebugStringA("Hello,DirectX!!\n");

	WindowsApp* win = nullptr;

	win = new WindowsApp();
	win->Initalize();
	MSG msg{};

#pragma region  DirectX����������

#pragma endregion

	//�Q�[�����[�v
	while (true) {
		
		if (win->gameloopExit(msg) == true) {
			break;
		}
#pragma region DirectX���t���[������

#pragma endregion

	}


	win->Release();

	delete win;
	return 0;
}