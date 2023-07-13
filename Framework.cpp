#include "Framework.h"

void Framework::Initialize()
{
	//windowsAPI������
	windows_->Initalize();

	//DirectX������
	directX_->Initialize(windows_.get());

	//keyborad������
	keyboard_->Initialize(windows_->GetHInstancee(), windows_->GetHwnd());

	gamePad_->Update();

	imGuiM_->Initialize(windows_.get(), directX_.get());
}

void Framework::Finalize()
{
}

void Framework::Update()
{
}

void Framework::IsEnd()
{
}

void Framework::Run()
{
	//�Q�[���̏�����
	Initialize();


	while (true)
	{
		IsEnd();

		//���t���[���X�V
		Update();

		if (GetIsEnd()) {
			break;
		}

		//�`��
		Draw();
	}


	//�Q�[���̏I��
	Finalize();
}
