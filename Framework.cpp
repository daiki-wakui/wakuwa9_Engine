#include "Framework.h"

void Framework::Initialize()
{
	//windowsAPI初期化
	windows_->Initalize();

	//DirectX初期化
	directX_->Initialize(windows_.get());

	//keyborad初期化
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
	//ゲームの初期化
	Initialize();


	while (true)
	{
		IsEnd();

		//毎フレーム更新
		Update();

		if (GetIsEnd()) {
			break;
		}

		//描画
		Draw();
	}


	//ゲームの終了
	Finalize();
}
