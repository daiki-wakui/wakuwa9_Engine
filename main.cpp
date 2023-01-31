#include "WindowsApp.h"
#include "KeyBoard.h"
#include "DirectXBasis.h"
#include "Object3D.h"
#include "Model.h"
#include "DirectionalLight.h"
#include "LightGroup.h"

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

	Object3D::StaticInitialize(dxBasis->GetDevice(), winApp->GetWindowWidth(), winApp->GetWindowHeight());

	//DirectionalLight::StaticInitalize(dxBasis->GetDevice());
	LightGroup::StaticInitialize(dxBasis->GetDevice());

	//keyborad������
	input_->Initialize(winApp->GetHInstancee(), winApp->GetHwnd());
	keyboard.reset(input_);

	//DirectionalLight* light = nullptr;
	LightGroup* lightGroup = nullptr;

	//���C�g����
	lightGroup = LightGroup::Create();
	//3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3D::SetLightGroup(lightGroup);

	//���s����
	float ambientColor0[3] = { 1,1,1 };

	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	//�_����
	float pointLightPos[3] = { 0,0,0 };
	float pointLightColor[3] = { 1,1,1 };
	float pointLightAtten[3] = { 0.3f,0.1f,0.1f };

	lightGroup->SetDirLightActive(0, false);
	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);

	/*lightGroup->SetPointLightActive(0, true);
	pointLightPos[0] = 0.5f;
	pointLightPos[1] = 1.0f;
	pointLightPos[2] = 0.0f;*/

	//�X�|�b�g���C�g
	float spotLightDir[3] = { 0,-1,0 };
	float spotLightPos[3] = { 0,5,0 };
	float spotLightColor[3] = { 1,1,1 };
	float spotLightAtten[3] = { 0.0f,0.0f,0.0f };
	float spotLightFactorAngle[2] = { 20.0f,30.0f};

	/*lightGroup->SetPointLightActive(0, false);
	lightGroup->SetPointLightActive(1, false);
	lightGroup->SetPointLightActive(2, false);
	lightGroup->SetSpotLightActive(0, true);*/

	//�ۉe
	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowAtten[3] = { 0.5f,0.0f,0.0f };
	float circleShadowFactorAngle[2] = { 1.0f,2.0f };

	float fightPos[3] = { 10,0.0f,5 };

	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, true);
	lightGroup->SetCircleShadowActive(0, true);

	//OBJ���烂�f����ǂݍ���
	Model* model = Model::LoadFromObj("sphere",true);
	Model* model3 = Model::LoadFromObj("floor");
	Model* model2 = Model::LoadFromObj("world");
	Model* model4 = Model::LoadFromObj("player");

	//3D�I�u�W�F�N�g����
	Object3D* object3d = Object3D::Create(5.0f);
	Object3D* object3d2 = Object3D::Create(2.0f);
	Object3D* object3d3 = Object3D::Create(100.0f);

	Object3D* objectFloor = Object3D::Create(5.0f);

	//3D�I�u�W�F�N�g��3D���f����R�Â�
	object3d->SetModel(model);
	object3d2->SetModel(model4);
	object3d3->SetModel(model2);

	object3d->SetPosition({ -10,0,+5 });
	object3d2->SetPosition(XMFLOAT3(fightPos));

	objectFloor->SetModel(model3);
	objectFloor->SetPosition({ 0,-10,0 });

	//�Q�[�����[�v
	while (true) {
		//�~�{�^���ŏI�����b�Z�[�W��������
		if (winApp->gameloopExit(msg) == true) {
			break;	//�Q�[�����[�v�I��
		}
		object3d->Update();
		object3d2->Update();
		object3d3->Update();
		objectFloor->Update();

		//keyborad�X�V����
		input_->Update();

		//light->Update();
		lightGroup->Update();

		//��������������				  �� ��
		static XMVECTOR lightDir = { 0,1,5,0 };

		if (input_->keyPush(DIK_RIGHT)) {
			fightPos[0]++;
		}
		else if (input_->keyPush(DIK_LEFT)) {
			fightPos[0]--;
		}
		if (input_->keyPush(DIK_UP)) {
			fightPos[1]++;
		}
		else if (input_->keyPush(DIK_DOWN)) {
			fightPos[1]--;
		}
		
		/*lightGroup->SetAmbientColor(XMFLOAT3(ambientColor0));
		lightGroup->SetDirLightDir(0, XMVECTOR({ lightDir0[0],lightDir0[1],lightDir0[2],0 }));
		lightGroup->SetDirLightColor(0, XMFLOAT3(lightColor0));
		lightGroup->SetDirLightDir(1, XMVECTOR({ lightDir1[0],lightDir1[1],lightDir1[2],0 }));
		lightGroup->SetDirLightColor(1, XMFLOAT3(lightColor1));
		lightGroup->SetDirLightDir(2, XMVECTOR({ lightDir2[0],lightDir2[1],lightDir2[2],0 }));
		lightGroup->SetDirLightColor(2, XMFLOAT3(lightColor2));*/


		/*lightGroup->SetPointLightPos(0, XMFLOAT3(pointLightPos));
		lightGroup->SetPointLightColor(0, XMFLOAT3(pointLightColor));
		lightGroup->SetPointLightAtten(0, XMFLOAT3(pointLightAtten));*/


		/*lightGroup->SetSpotLightDir(0, XMVECTOR({ spotLightDir[0],spotLightDir[1],spotLightDir[2] ,0 }));
		lightGroup->SetSpotLightPos(0, XMFLOAT3({ spotLightPos }));
		lightGroup->SetSpotLightColor(0, XMFLOAT3({ spotLightColor }));
		lightGroup->SetSpotLightAtten(0, XMFLOAT3({ spotLightAtten }));
		lightGroup->SetSpotLightFactorAngle(0, XMFLOAT2(spotLightFactorAngle));*/

		lightGroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0],circleShadowDir[1],circleShadowDir[2] ,0 }));
		lightGroup->SetCircleShadowCasterPos(0, XMFLOAT3({ fightPos[0],fightPos[1],fightPos[2] }));
		lightGroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
		lightGroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));

		object3d2->SetPosition(XMFLOAT3({ fightPos[0], fightPos[1], fightPos[2] }));

		XMFLOAT3 pos3d;
		XMFLOAT3 pos3d2;

		pos3d = object3d->GetRotation();
		pos3d2 = object3d2->GetPosition();

		pos3d.y++;

		object3d->SetRotation(pos3d);
		object3d2->SetRotation(pos3d);

#pragma region DirectX���t���[������

		// �`��O����
		dxBasis->PreDraw();

		Object3D::PreDraw(dxBasis->GetCommandList());

		object3d->Draw();
		object3d2->Draw();
		object3d3->Draw();
		objectFloor->Draw();

		Object3D::PostDraw();

		//�`��㏈��
		dxBasis->PostDraw();


		
#pragma endregion

	}

	//delete light;
	delete lightGroup;
	delete model;
	delete model2;
	delete model3;
	delete model4;
	delete object3d;
	delete object3d2;
	delete object3d3;
	delete objectFloor;
	//�E�B���h�E�N���X��o�^����
	winApp->Release();

	return 0;
}
