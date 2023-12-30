#pragma once
#include "TitleScene.h"
#include "GameScene.h"

#include <memory>

/**
 * @file SceneManager
 * @brief シーン管理しているクラス
 */

class SceneManager
{
private:

	//キーボードクラスとゲームパッドを使えるように
	KeyBoard* keyboard_ = KeyBoard::GetInstance();
	GamePad* gamePad_ = GamePad::GetInstance();

	//シーンの追加
	std::unique_ptr<GameScene> gamescene_ = std::make_unique<GameScene>();
	std::unique_ptr<TitleScene> titlescene_ = std::make_unique<TitleScene>();

	//現在のシーン
	int32_t nowScene_ = 0;

public:

	//デストラクタ
	~SceneManager();

	//初期化
	void Initialize();
	//更新処理
	void Update();
	//描画関数
	void Draw();
	//後始末
	void Finalize();
	//パーティクル描画
	void ParticleDraw();
	//ポストエフェクトをかけない描画
	void OffEffectDraw();

	//ゲームシーンに遷移するかの関数
	bool ChangeToGameScene();
	//タイトルシーンに遷移するかの関数
	bool ChangeToTitleScene();

public: //getter

	int32_t GetSceneState() { return nowScene_; }
	GameScene* GetGameScene() { return gamescene_.get(); }
	TitleScene* GetTitleScene() { return titlescene_.get(); }
};

