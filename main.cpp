#include "WindowsApp.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//windowsAPI�̐����N���X
	WindowsApp* win = nullptr;
	win = new WindowsApp();

	////windowsAPI������
	win->Initalize();
	MSG msg{};

#pragma region  DirectX����������

#pragma endregion

	//�Q�[�����[�v
	while (true) {
		//�~�{�^���ŏI�����b�Z�[�W��������
		if (win->gameloopExit(msg) == true) {
			break;	//�Q�[�����[�v�I��
		}
#pragma region DirectX���t���[������

#pragma endregion

	}

	//�E�B���h�E�N���X��o�^����
	win->Release();
	//���
	delete win;
	return 0;
}