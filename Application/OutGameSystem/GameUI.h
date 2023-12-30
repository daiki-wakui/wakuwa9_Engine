#pragma once
#include "TextureManager.h"
#include "Sprite.h"
#include "Player.h"
#include "Boss.h"
#include "GamePad.h"
#include "Object3D.h"
#include "Vector2.h"
#include "Vector3.h"

class GameUI : public TextureManager
{
private:

	//ゲームパッド処理使えるように
	GamePad* gamePad_ = GamePad::GetInstance();

	//借りてくる情報
	Player* player_ = nullptr;
	Object3D* playerObject_ = nullptr;
	Boss* boss_ = nullptr;

	//タイトルシーンで使うスプライト
	std::unique_ptr<Sprite> titleSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> titleUISprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> fillSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> sceneSprite_ = std::make_unique<Sprite>();

	//タイトルシーンからゲームシーンに遷移するときに使うアルファ値
 	float ChangeTitleAlpha_ = 0;

	//ゲームシーンで使うスプライト
	std::unique_ptr<Sprite> playerHPSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> bossHPSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> gameoverSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> gameclearSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> reticleSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> sSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> iventSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> RBSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> dFilterSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> bulletRreticleSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> waringSprite_ = std::make_unique<Sprite>();

	//ダメージシェイク値
	Vector3 randShake_;

	//イベントシーンで使うスプライトのアルファ値
	float iventAlpha_ = 0;

	float alphaRB_ = 0;

	//シーン遷移のフェード用
	float ChangeGameAlpha_ = 1;

	//レティクルUI
	DirectX::XMFLOAT2 reticleSize_ = { 0,720 };

	//スクリーン上でのプレイヤーの座標
	Vector3 screenPosPlayer_ = {};

	bool hitBox_;
	bool isIvent_;
	bool isManual_;

	//危険な情報を知らせるときのUIに使う変数
	bool isLifeOne_ = false;
	float fillAlpha_ = 0;
	int32_t fillTimer_ = 0;


	int32_t pow_ = 0;
	int32_t count_ = 0;
	float wTimer_ = 0;
	float wMax_ = 10;
	float popFrame_ = 0;

	Vector3 wSize_ = {};
	bool movieEnd_ = false;
public:

	//コンストラクタ、デストラクタ
	GameUI();
	~GameUI();

	//リセット関数
	void Reset();

	//タイトルシーンUI
	//初期化
	void TitleSceneInitialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="sceneChange"></param> シーン遷移中
	void TitleUpdate(bool sceneChange);

	//描画関数
	void TitleDraw();

	//ゲームシーンUI
	//初期化
	void GameSceneInitialize();

	//更新処理
	void GameUpdate();

	//描画関数
	void GameDraw();

	//ポストエフェクトをかけないスプライトの描画
	void OffDraw();

	//ダメージシェイク時の処理
	void Shake();

	//BossHPUIの更新処理
	void BossHpUI();

	//ゲームシーンのシーン遷移のフェード用関数
	void SceneStartFadeUI();

	//ボス戦のイベントムービー用の関数
	void BossIventSceneUI();

public:	//getter,setter

	//シェイクの値
	Vector3 GetRandShake() { return randShake_; }
	//アルファ値を取得
	float GetSceneChangeAlpha() { return ChangeTitleAlpha_; }
	float GetGameSceneChangeAlpha() { return ChangeGameAlpha_; }

	void SetMovieEnd(bool movieEnd) { movieEnd_ = movieEnd; }

	//必要な情報を借りてくる
	void SetInfo(Player* player, Object3D* playerObject, Boss* boss);
	void boolInfo(bool hitBox, bool isIvent);
};

