#pragma once
#include "Framework.h"
#include "BaseScene.h"

#include "Model.h"
#include "Sprite.h"
#include "Sound.h"

class TitleScene : public BaseScene
{
private: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:

	DirectXBasis* directX_ = nullptr;
	SpriteBasis* spBasis_ = nullptr;
	WindowsApp* windows_ = nullptr;
	ImGuiManager* imGuiM_ = nullptr;
	Sound* sound_ = nullptr;

	KeyBoard* keyboard_ = nullptr;
	GamePad* gamePad_ = nullptr;

	std::unique_ptr<Sprite> titleSprite_ = std::make_unique<Sprite>();

	std::unique_ptr<Sprite> fillSprite_ = std::make_unique<Sprite>();


	std::unique_ptr<Model> skydomModel_;
	std::unique_ptr<Object3D> skyObject_;

	//画像
	int32_t tex1_ = 0;

	int32_t title_ = 0;

	int32_t scene_ = 0;

	int32_t fillter_ = 0;

	XMFLOAT2 pos = {0,0};
	float power = 1;

	bool start_ = false;
	bool change_ = false;

	std::unique_ptr<Sprite> sceneSprite_ = std::make_unique<Sprite>();

	bool playBGM_ = false;

public:

	bool GetChange() { return change_; }
	void SetChange(bool change) { change_ = change; }
	bool GetStart() { return start_; }
	void SetStart(bool start) { start_ = start; }

	void SetBasis(WindowsApp* windows, DirectXBasis* directX, ImGuiManager* imGuiM, SpriteBasis* spBasis, Sound* sound_);
	void SetInputInfo(KeyBoard* keyboard, GamePad* gamePad);

	//初期化
	void Initialize() override;

	//終了
	void Finalize()override;

	//毎フレーム
	void Update()override;

	//描画
	void Draw()override;

	void OffDraw();
};

