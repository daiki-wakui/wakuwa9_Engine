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

	//windowsAPI�̐����N���X
	std::unique_ptr<WindowsApp> windows;
	WindowsApp* winApp = new WindowsApp();

	//keyborad�N���X�̐���
	std::unique_ptr<KeyBoard> keyboard;
	KeyBoard* input_ = new KeyBoard();

	//DirectX�̊�Ր����N���X
	std::unique_ptr<DirectXBasis> DirectX;
	DirectXBasis* dxBasis = new DirectXBasis();

	//Sprite�̊�Ր����N���X
	std::unique_ptr<SpriteBasis> SpBasis;
	SpriteBasis* spBasis = new SpriteBasis();


#pragma region  ��Օ����̏�����

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

	spBasis->Initialize(dxBasis,0);
	SpBasis.reset(spBasis);

#pragma endregion	

#pragma region  �`�揉��������
	
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

	//�Q�[�����[�v
	while (true) {
		//�~�{�^���ŏI�����b�Z�[�W��������
		if (winApp->gameloopExit(msg) == true) {
			break;	//�Q�[�����[�v�I��
		}

		//keyborad�X�V����
		input_->Update();

#pragma region  DirectX���t���[���X�V����
		

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

#pragma region DirectX���t���[���`�揈��
		// �`��O����
		dxBasis->PreDraw();

		sprite->Draw();
		//sprite2->Draw();

		//�`��㏈��
		dxBasis->PostDraw();
#pragma endregion
	}
	//�E�B���h�E�N���X��o�^����
	winApp->Release();
	delete sprite;
	return 0;
}