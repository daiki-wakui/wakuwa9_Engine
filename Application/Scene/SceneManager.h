#pragma once
#include "BaseScene.h"

#include <memory>

class SceneManager
{
private:

	//キーボードクラスとゲームパッドを使えるように
	KeyBoard* keyboard_ = KeyBoard::GetInstance();
	GamePad* gamePad_ = GamePad::GetInstance();

	//シーンの追加
	/*std::unique_ptr<GameScene> gamescene_ = std::make_unique<GameScene>();
	std::unique_ptr<TitleScene> titlescene_ = std::make_unique<TitleScene>();*/

	//現在のシーン
	int32_t nowScene_ = 1;



	BaseScene* scene_ = nullptr;
	BaseScene* nextScene_ = nullptr;

private:


	SceneManager() = default;
	~SceneManager();

	//コピーコンストラクタ無効
	SceneManager(const SceneManager& obj) = delete;
	//代入演算子を無効
	SceneManager& operator=(const SceneManager& obj) = delete;

public:
	static SceneManager* GetInstance();

	void SetNextScene(BaseScene* nextScene) { nextScene_ = nextScene; }

	
	void Initialize();
	void Update();
	void Draw();
	void Finalize();
	void ParticleDraw();
	void OffEffectDraw();

	//タイトルシーンからゲームシーンに変わる時
	bool ChangeToGameScene();
	bool ChangeToTitleScene();

public: //getter

	int32_t GetSceneState() { return nowScene_; }
	/*GameScene* GetGameScene() { return gamescene_.get(); }
	TitleScene* GetTitleScene() { return titlescene_.get(); }*/
};

