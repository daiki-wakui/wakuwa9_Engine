#pragma once
#include "Framework.h"
#include "BaseScene.h"
#include "GameUI.h"
#include "Model.h"
#include "Vector3.h"
#include "constJsonValue.h"

#include "json.hpp"
#include <fstream>
#include <iostream>

/**
 * @file TitleScene
 * @brief タイトルシーンを動かしているクラス
 */

class TitleScene : public BaseScene
{
private: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:

	DirectXBasis* directX_ = DirectXBasis::GetInstance();
	SpriteBasis* spBasis_ = SpriteBasis::GetInstance();
	WindowsApp* windows_ = WindowsApp::GetInstance();
	ImGuiManager* imGuiM_ = ImGuiManager::GetInstance();
	Sound* sound_ = Sound::GetInstance();

	KeyBoard* keyboard_ = KeyBoard::GetInstance();
	GamePad* gamePad_ = GamePad::GetInstance();

private:	//メンバ変数
	std::unique_ptr<constJsonValue> json_;

	//天球
	std::unique_ptr<Object3D> skyObject_;
	std::unique_ptr<GameUI> titleUI_ = std::make_unique<GameUI>();

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
	void Initialize();

	//終了
	void Finalize();

	//更新処理
	void Update();

	//描画
	void Draw();

	//ポストエフェクトをかけない描画
	void OffDraw();

public:	//setter,getter

	bool GetChange() { return changeEnd_; }
	void SetChange(bool change) { changeEnd_ = change; }
	bool GetStart() { return changeStart_; }
	void SetStart(bool start) { changeStart_ = start; }
};

