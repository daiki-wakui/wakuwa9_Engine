#pragma once
#include "Framework.h"
#include "BaseScene.h"
#include "GameUI.h"

#include "EventBox.h"
#include "Enemy.h"
#include "Door.h"

#include "LevelLoader.h"

#include "Effect.h"
#include "DebugPoint.h"

#include <memory>
#include <list>

#include "BaseObject.h"
#include "GameObjectList.h"

#include "LevelEditor.h"
#include "GameCollisons.h"

/**
 * @file GameScene
 * @brief ゲームシーンを動かしているクラス
 */

class GameScene : public BaseScene
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
	
private: //定数

	const float MAX_SHACK_TIME = 30.0f;
	const float SHADOW_POS_Y = 2.0f;
	const Vector3 SHADOW_SCALE = { 1.5f, 1.5f, 1.5f};
	const float BULLET_SHOT_VOLUE = 0.25f;
	const float WARNING_VOLUE = 2.0f;
	const float NOISE_SE_VOLUE = 0.5f;
	const float SHOCK_SE_VOLUE = 1.1f;
	const float HIT_SE_VOLUE = 0.12f;
	const Vector3 SHOT_EFFECT_SCALE = { 5.0f, 5.0f, 5.0f };
	const Vector3 SHOT_EFFECT_POS = { 0.0f, 10.0f, 10.0f };
	const Vector3 SKY_SCALE = { 900.0f, 900.0f, 900.0f };
	const Vector3 SKY_POS = { 0.0f, 0.0f, 100.0f };


	const int32_t COOLTIME_NUM = 2;
	const int32_t EFFECT_NUM = 10;
	const float EFFECT_Y = 1.5f;

	const float BIT_ROT_VOLUE_Y = 180.0f;
	const float BIT_ROT_VOLUE_Z = 225.0f;

	const float EVENT_TIME_FRAME = 60.0f;
	const Vector3 LNIT_EYE = { 0.0f,20.0f,-30.0f };
	const Vector3 LNIT_EVENT_EYE = { 450.0f,100.0f,750.0f };
	const Vector3 LNIT_TERGET = { 0.0f,10.0f,0.0f };
	const Vector3 PLAYER_SCALE = { 1.0f,1.0f,1.0f };
	const Vector3 BOSS_SCALE = { 15.0f,15.0f,15.0f };
	const Vector3 BOSS_POS = { 0.0f,20.0f,370.0f };

	const float DOOR_POS_VOLUE_X = 15.0f;

	const float COL_PLAYER_SCALE = 6.0f;
	const float COL_ENEMY_SCALE = 7.0f;
	const float COL_EVENTBOX_SCALE = 13.0f;
	const float COL_BOSS_SCALE = 20.0f;
	const float COL_BOSS_TAIL_SCALE = 20.0f;
	const float COL_BULLET_SCALE = 1.0f;
	const float COL_SEARCH_ENEMY_SCALE = 175.0f;


private:	//メンバ変数

	BaseObject* playerObj_;

	std::unique_ptr<GameCollisons> col_ = std::make_unique<GameCollisons>();
	
	std::list<std::unique_ptr<BaseObject>> gameObjects_;
	std::unique_ptr<BaseObject> baseObject_[1000] = {};
	int32_t objNum_ = 0;

	//OBJからモデルを読み込む
	//天球
	std::unique_ptr<Object3D> skyObject_;
	std::unique_ptr<Object3D> poriObject_;
	std::unique_ptr<Object3D> shadowObject_;

	//player,肩のポッド
	std::unique_ptr<Object3D> playerObject_;
	std::unique_ptr<Object3D> podObject_;
	std::unique_ptr<Object3D> bulletObject_;

	//ボス
	std::unique_ptr<Object3D> bossObject_;	

	Vector3 podRot = {};
	int32_t sockSound_ = 0;

	std::unique_ptr<GameUI> gameUI_ = std::make_unique<GameUI>();

	//player
	std::unique_ptr<Player> player_ = std::make_unique<Player>();

	//boss
	std::unique_ptr<Boss> boss_ = std::make_unique<Boss>();

	
	//パーティクル
	std::unique_ptr<ParticleManager> particleMan_ = std::make_unique<ParticleManager>();
	bool BulletEffect = false;

	bool start_ = false;
	bool change_ = false;
	XMFLOAT2 pos = { 0,0 };
	
	bool bossBGM_ = false;
	
	Vector3 screenPos_ = {};
	XMFLOAT2 spPos_ = {};

	float alpha_ = 0;
	bool isIvent_ = false;

	Vector3 iventEye_ = { 450,100,750 };
	Vector3 iventTarget_ = { 450, 100,1000 };

	Vector3 endEye_ = { 450,10,600 };
	float timer_ = 0;
	float maxTime_ = 300;

	float addYRB_ = 0;
	XMFLOAT2 posRB_ = {};

	std::list<std::unique_ptr<Effect>> effects_;
	bool isEffect_ = 0;
	Vector3 startEffect_ = {};

	int32_t coolTime_ = 7;
	bool isShotEffect_ = false;
	
	bool resetOn_ = false;

	bool hitBox_ = true;
	bool isTutorial_ = false;
	bool isChangeStage_ = false;
	
	bool isDebugBoss_ = false;
	bool isShake_;
	int32_t shakeTimer_;

	Model3DManager* m = Model3DManager::GetInstance();

	std::unique_ptr<LevelEditor> lv = std::make_unique<LevelEditor>();

public:
	//3dオブジェクト生成
	void Object3DGenerate();

	//初期化
	void Initialize()override;

	//終了
	void Finalize()override;

	//更新処理
	void Update()override;
	//スプライト更新処理
	void SpriteUpdate();
	//スプライト描画
	void SpriteDraw();

	//3dオブジェクト更新処理
	void ObjectUpdate();

	//描画
	void Draw()override;

	//パーティクル描画
	void ParticleDraw();

	//シーンリセット関数
	void Reset();

public:	//getter,setter
	Player* GetPlayer() { return player_.get(); }
	Boss* GetBoss() { return boss_.get(); }

	bool GetChange() { return change_; }
	void SetChange(bool change) { change_ = change; }
	bool GetStart() { return start_; }
	void SetStart(bool start) { start_ = start; }

	bool GetHitBox() { return hitBox_; }
	bool GetIsIvent() { return isIvent_; }

	const std::list<std::unique_ptr<BaseObject>>& GetObjList() { return gameObjects_; }
};

