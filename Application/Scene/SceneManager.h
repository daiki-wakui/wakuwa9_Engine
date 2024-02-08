#pragma once
#include "BaseScene.h"
#include "AbstactSceneFactory.h"

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

	//現在のシーン
	int32_t nowScene_ = 0;

	BaseScene* scene_ = nullptr;
	BaseScene* nextScene_ = nullptr;

	AbstractSceneFactory* sceneFactory_ = nullptr;

public:
	void SetNextScene(BaseScene* nextScene) { nextScene_ = nextScene; }
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }

	void ChangeScene(const std::string& sceneName);

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

};

