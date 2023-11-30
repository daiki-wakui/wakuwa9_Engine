#pragma once
#include "TextureManager.h"
#include "Sprite.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Player.h"
#include "GamePad.h"
#include "Object3D.h"

class GameUI : public TextureManager
{
private:

	//ゲームパッド処理使えるように
	GamePad* gamePad_ = GamePad::GetInstance();

	//借りてくる情報
	Player* player_ = nullptr;
	Object3D* playerObject_ = nullptr;

	//タイトルシーンで使うスプライト
	std::unique_ptr<Sprite> titleSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> titleUISprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> fillSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> sceneSprite_ = std::make_unique<Sprite>();

	//タイトルシーンからゲームシーンに遷移するときに使うアルファ値
 	float ChangeAlpha_ = 0;

	//ゲームシーンで使うスプライト
	std::unique_ptr<Sprite> playerHPSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> bossHPSprite_ = std::make_unique<Sprite>();
//	std::unique_ptr<Sprite> titleSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> gameoverSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> gameclearSprite_ = std::make_unique<Sprite>();
//	std::unique_ptr<Sprite> fillSprite_ = std::make_unique<Sprite>();
//	std::unique_ptr<Sprite> sceneSprite_ = std::make_unique<Sprite>();
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

	//レティクルUI
	DirectX::XMFLOAT2 reticleSize_ = { 0,720 };

	//スクリーン上でのプレイヤーの座標
	Vector3 screenPosPlayer_ = {};

public:

	GameUI();
	~GameUI();

	//タイトルシーンUI
	void TitleSceneInitialize();
	void TitleUpdate(bool sceneChange);
	void TitleDraw();

	//ゲームシーンUI
	void GameSceneInitialize();
	void GameUpdate();
	void GameDraw();

	//ポストエフェクトをかけないスプライトの描画
	void OffDraw();

	//ダメージシェイク時の処理
	void Shake();

public:	//getter,setter

	//シェイクの値
	Vector3 GetRandShake() { return randShake_; }
	//アルファ値を取得
	float GetSceneChangeAlpha() { return ChangeAlpha_; }

	//必要な情報を借りてくる
	void SetInfo(Player* player, Object3D* playerObject);
};

