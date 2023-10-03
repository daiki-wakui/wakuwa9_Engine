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

private:	//メンバ変数
	//天球
	std::unique_ptr<Model> skydomModel_;
	std::unique_ptr<Object3D> skyObject_;

	//スプライト
	std::unique_ptr<Sprite> titleSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> titleUISprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> fillSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> sceneSprite_ = std::make_unique<Sprite>();

	//画像
	int32_t titleImage_ = 0;
	int32_t titleUIImage_ = 0;
	int32_t sceneChangeImage_ = 0;
	int32_t filterImage_ = 0;

	int32_t testImage_ = 0;


	//シーン遷移に使う値
	XMFLOAT2 changeSize_ = {0,0};
	float addSize_ = 1;
	bool changeStart_ = false;
	bool changeEnd_ = false;

	//BGM
	bool playBGM_ = false;

	float ChangeAlpha_ = 0;
	bool isStartSE_ = false;

public:	//メンバ関数

	//初期化
	void Initialize() override;

	//終了
	void Finalize()override;

	//毎フレーム
	void Update()override;

	//描画
	void Draw()override;

	void OffDraw();

public:	//setter,getter

	bool GetChange() { return changeEnd_; }
	void SetChange(bool change) { changeEnd_ = change; }
	bool GetStart() { return changeStart_; }
	void SetStart(bool start) { changeStart_ = start; }

	void SetBasis(WindowsApp* windows, DirectXBasis* directX, ImGuiManager* imGuiM, SpriteBasis* spBasis, Sound* sound_);
	void SetInputInfo(KeyBoard* keyboard, GamePad* gamePad);

};

