#include "WindowsApp.h"
#include "KeyBoard.h"
#include "DirectXBasis.h"
#include <memory>
#include <string>
#include <DirectXTex.h>

#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//windowsAPI�̐����N���X
	std::unique_ptr<WindowsApp> windows;
	WindowsApp* winApp = new WindowsApp();

	//keyborad�N���X�̐���
	std::unique_ptr<KeyBoard> keyboard;
	KeyBoard* input_ = new KeyBoard();

	//DirectX�̊�Ր����N���X
	std::unique_ptr<DirectXBasis> DirectX;
	DirectXBasis* dxBasis = new DirectXBasis();

	//windowsAPI������
	winApp->Initalize();
	windows.reset(winApp);
	MSG msg{};

	//DirectX������
	dxBasis->Initialize(winApp);
	DirectX.reset(dxBasis);

	//keyborad������
	input_->Initialize(winApp->GetHInstancee(), winApp->GetHwnd());
	keyboard.reset(input_);

#pragma region  �`�揉��������

#pragma endregion

	//�Q�[�����[�v
	while (true) {
		//�~�{�^���ŏI�����b�Z�[�W��������
		if (winApp->gameloopExit(msg) == true) {
			break;	//�Q�[�����[�v�I��
		}

		//keyborad�X�V����
		input_->Update();

#pragma region DirectX���t���[������

		// �`��O����
		dxBasis->PreDraw();

		//�`��㏈��
		dxBasis->PostDraw();
#pragma endregion

	}

	//�E�B���h�E�N���X��o�^����
	winApp->Release();

	return 0;
}